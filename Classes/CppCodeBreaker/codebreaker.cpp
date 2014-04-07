//---------------------------------------------------------------------------
/*
CodeBreaker, code breaking class
Copyright (C) 2010-2014 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
// From http://www.richelbilderbeek.nl/CppCodeBreaker.htm
//---------------------------------------------------------------------------
#include "codebreaker.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <numeric>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/math/distributions/chi_squared.hpp>

#include "caesarcipher.h"
#include "trace.h"
#include "loopreader.h"
#include "vigenerecipher.h"
#pragma GCC diagnostic pop

ribi::CodeBreaker::CodeBreaker()
{
  #ifndef NDEBUG
  Test();
  #endif
}

std::pair<double,double> ribi::CodeBreaker::CalculateChiSquared(
  const std::map<char,double>& frequency_measured,
  const std::map<char,double>& frequency_expected
) const noexcept
{
  const bool verbose = false;
  std::vector<double> tally_expected;
  std::vector<double> tally_measured;

  for (char c = 'a'; c<='z'; ++c)
  {
    tally_measured.push_back(
      frequency_measured.count(c) == 0
      ? 0.0
      : frequency_measured.find(c)->second
    );
    tally_expected.push_back(
      frequency_expected.count(c) == 0
      ? 0.0
      : frequency_expected.find(c)->second
    );
  }
  const std::vector<double> rel_error = CalculateRelativeError(tally_measured,tally_expected);
  const int n_categories = 26;
  assert(n_categories == static_cast<int>(tally_measured.size()));
  assert(n_categories == static_cast<int>(tally_expected.size()));
  assert(n_categories == static_cast<int>(rel_error.size()));
  if (verbose)
  {
    for (std::size_t i=0; i!=n_categories; ++i)
    {
      std::cout
        << tally_measured[i] << "\t"
        << tally_expected[i] << "\t"
        << rel_error[i] << "\n";
    }
  }

  const double significance_level = 0.05;
  const double chi_squared_value
    = std::accumulate(rel_error.begin(),rel_error.end(),0.0);

  const double degrees_of_freedom
    = static_cast<double>(n_categories)
    - 1.0 //We need to calculate the mean ourselves
    - 1.0 //We need to calculate the standard deviation ourselves
    - 1.0; //We need to calculate the sample size ourselves

  boost::math::chi_squared_distribution<double> distribution(degrees_of_freedom);
  const double critical_value
    = boost::math::quantile(boost::math::complement(distribution, significance_level));
  if (verbose)
  {
    std::cout
      //<< "Mean size: " << mean
      //<< "\nStdDev size: " << stdDev
      << "\nSUM observer: "
        << std::accumulate(tally_measured.begin(),tally_measured.end(), 0)
      << "\nSUM expected: "
        << std::accumulate(tally_expected.begin(),tally_expected.end(),0.0)
      << "\nChi-square value: " << chi_squared_value
      << "\nSignificance level: " << significance_level
      << "\nDegrees of freedom: " << degrees_of_freedom
      << "\nCritical value: " << critical_value << '\n';
  }
  return std::make_pair(chi_squared_value,critical_value);
}

std::vector<double> ribi::CodeBreaker::CalculateRelativeError(
  const std::vector<double>& frequency_measured,
  const std::vector<double>& frequency_expected
) const noexcept
{
  assert(frequency_measured.size() == frequency_expected.size());
  const std::size_t sz = frequency_measured.size();
  std::vector<double> v(sz);
  for (std::size_t i = 0; i!=sz; ++i)
  {
    const double obs = frequency_measured[i];
    const double exp = frequency_expected[i];
    assert(exp!=0.0);
    v[i] = ((obs-exp)*(obs-exp))/exp;
  }
  return v;
}

std::map<char,double> ribi::CodeBreaker::GetCharFrequency(const std::string& text) const noexcept
{
  const auto m(GetCharTally(text));
  const int sum = std::accumulate(m.begin(),m.end(),0,
    [](const int init, const std::pair<char,int>& p)
    {
      return init + p.second;
    }
  );
  assert(sum > 0);
  std::map<char,double> n;
  for (const auto p: m)
  {
    n.insert(std::make_pair(p.first, static_cast<double>(p.second) / static_cast<double>(sum)));
  }
  return n;
}

std::map<char,int> ribi::CodeBreaker::GetCharTally(const std::string& text) const noexcept
{
  std::map<char,int> m;
  for (const auto c:text)
  {
    if (m.count(c) == 0) m.insert(std::make_pair(c,0));
    ++m[c];
  }
  return m;
}

std::vector<std::map<char,int>> ribi::CodeBreaker::GetCharTally(const std::string& text, const int period) const noexcept
{
  assert(period > 0);
  std::vector<std::map<char,int>> v(period);
  const int length = static_cast<int>(text.size());

  for (int i=0; i!=length; ++i)
  {
    const char c = text[i];
    std::map<char,int>& m = v[i % period];
    if (m.count(c) == 0) m.insert(std::make_pair(c,0));
    {
      ++m[c];
    }
  }
  return v;
}

std::map<char,double> ribi::CodeBreaker::GetLetterFrequencyDutch() const noexcept
{
  std::map<char,double> m;
  m.insert(std::make_pair('a',0.0749));
  m.insert(std::make_pair('b',0.0158));
  m.insert(std::make_pair('c',0.0124));
  m.insert(std::make_pair('d',0.0593));
  m.insert(std::make_pair('e',0.1891));
  m.insert(std::make_pair('f',0.0081));
  m.insert(std::make_pair('g',0.0340));
  m.insert(std::make_pair('h',0.0238));
  m.insert(std::make_pair('i',0.0650));
  m.insert(std::make_pair('j',0.0146));
  m.insert(std::make_pair('k',0.0225));
  m.insert(std::make_pair('l',0.0357));
  m.insert(std::make_pair('m',0.0221));
  m.insert(std::make_pair('n',0.1003));
  m.insert(std::make_pair('o',0.0606));
  m.insert(std::make_pair('p',0.0157));
  m.insert(std::make_pair('q',0.00009));
  m.insert(std::make_pair('r',0.0641));
  m.insert(std::make_pair('s',0.0373));
  m.insert(std::make_pair('t',0.0679));
  m.insert(std::make_pair('u',0.0199));
  m.insert(std::make_pair('v',0.0285));
  m.insert(std::make_pair('w',0.0152));
  m.insert(std::make_pair('x',0.00040));
  m.insert(std::make_pair('y',0.00035));
  m.insert(std::make_pair('z',0.0139));
  return m;
}

std::map<char,double> ribi::CodeBreaker::GetLetterFrequencyEnglish() noexcept
{
  std::map<char,double> m;
  m.insert(std::make_pair('a',0.08167));
  m.insert(std::make_pair('b',0.01492));
  m.insert(std::make_pair('c',0.02782));
  m.insert(std::make_pair('d',0.04253));
  m.insert(std::make_pair('e',0.12702));
  m.insert(std::make_pair('f',0.02228));
  m.insert(std::make_pair('g',0.02015));
  m.insert(std::make_pair('h',0.06094));
  m.insert(std::make_pair('i',0.06966));
  m.insert(std::make_pair('j',0.00153));
  m.insert(std::make_pair('k',0.00772));
  m.insert(std::make_pair('l',0.04025));
  m.insert(std::make_pair('m',0.02406));
  m.insert(std::make_pair('n',0.06749));
  m.insert(std::make_pair('o',0.07507));
  m.insert(std::make_pair('p',0.01929));
  m.insert(std::make_pair('q',0.00095));
  m.insert(std::make_pair('r',0.05987));
  m.insert(std::make_pair('s',0.06327));
  m.insert(std::make_pair('t',0.09056));
  m.insert(std::make_pair('u',0.02758));
  m.insert(std::make_pair('v',0.00978));
  m.insert(std::make_pair('w',0.02360));
  m.insert(std::make_pair('x',0.00150));
  m.insert(std::make_pair('y',0.01974));
  m.insert(std::make_pair('z',0.00074));
  return m;
}

std::string ribi::CodeBreaker::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::CodeBreaker::GetVersionHistory() noexcept
{
  return {
    "2014-04-04: version 1.0: initial version"
  };
}

int ribi::CodeBreaker::GuessCaesarCipherKey(
  const std::string& secret_text,
  const std::map<char,double>& expected_char_frequency
  ) const noexcept
{
  const bool verbose = false;
  std::vector<double> v;

  for (int key=0; key!=26; ++key)
  {
    CaesarCipher c(key);
    const std::string plain_text = c.Deencrypt(secret_text);
    const auto p(
      CalculateChiSquared(
        GetCharFrequency(plain_text),
        expected_char_frequency
      )
    );
    v.push_back(p.first);
  }
  if (verbose)
  {
    for (int i=0; i!=26; ++i)
    {
      std::cout << i << ": " << v[i] << '\n';
    }
  }
  //The lower the value the better
  return std::distance(v.begin(),std::min_element(v.begin(),v.end()));
}

#ifndef NDEBUG
void ribi::CodeBreaker::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  const CodeBreaker b;
  //const bool verbose = false;
  TRACE("Starting ribi::CodeBreaker::Test");
  {
    const auto p(
      b.CalculateChiSquared(
        b.GetCharFrequency(b.GetExampleEnglish()),
        b.GetLetterFrequencyEnglish()
      )
    );
    const double chi_squared_value = p.first;
    const double critical_value = p.second;
    assert(chi_squared_value < critical_value
      && "Cannot reject null hypothesis that the measured values "
         "do follow the expected frequencies"
    );
  }
  {
    const auto p(
      b.CalculateChiSquared(
        b.GetCharFrequency(b.GetExampleDutch()),
        b.GetLetterFrequencyDutch()
      )
    );
    const double chi_squared_value = p.first;
    const double critical_value = p.second;
    assert(chi_squared_value < critical_value
      && "Cannot reject null hypothesis that the measured values "
         "do follow the expected frequencies"
    );
  }
  //Check that encrypted text has significantly different frequencies
  //which fails for Caesar cipher
  for (int i=0; i!=26; ++i)
  {
    CaesarCipher c(i);
    const std::string text = c.Clean(b.GetExampleEnglish());
    const std::string encrypted = c.Encrypt(text);
    //TRACE(encrypted);

    const auto p(
      b.CalculateChiSquared(
        b.GetCharFrequency(encrypted),
        b.GetLetterFrequencyEnglish()
      )
    );
    const double chi_squared_value = p.first;
    const double critical_value = p.second;
    assert(chi_squared_value < critical_value
      && "Cannot reject null hypothesis that the measured values "
         "do follow the expected frequencies"
         "(as a Ceasar cipher on a short text is not conclusive enough)"
    );
  }
  //Check that encrypted text has significantly different frequencies
  //which fails for Vigenere cipher
  for (int i=0; i!=26; ++i)
  {
    const int key_length = 1 + (i * i);
    std::string key;
    for (int j=0; j!=key_length; ++j)
    {
      key += 'a' +( std::rand() % 26);
    }
    VigenereCipher c(key);
    const std::string text = c.Clean(b.GetExampleEnglish());
    const std::string encrypted = c.Encrypt(text);
    //TRACE(encrypted);

    const auto p(
      b.CalculateChiSquared(
        b.GetCharFrequency(encrypted),
        b.GetLetterFrequencyEnglish()
      )
    );
    const double chi_squared_value = p.first;
    const double critical_value = p.second;
    assert(chi_squared_value < critical_value
      && "Cannot reject null hypothesis that the measured values "
         "do follow the expected frequencies"
         "(as a Vigenere cipher on a short text is not conclusive enough)"
    );
  }
  //Guess the Ceasar cipher key
  for (int key=0; key!=26; ++key)
  {
    CaesarCipher c(key);
    const std::string text = CaesarCipher::Clean(b.GetExampleEnglish());
    const std::string secret_text = c.Encrypt(text);
    assert(b.GuessCaesarCipherKey(secret_text) == key);
  }
  TRACE("Finished ribi::CodeBreaker::Test successfully");
}
#endif

