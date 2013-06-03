#ifdef _WIN32
//See http://www.richelbilderbeek.nl/CppCompileErrorSwprintfHasNotBeenDeclared.htm
#undef __STRICT_ANSI__
#endif

//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "lsqfilter.h"

#include <cassert>
#include <cstdlib>

LsqFilter::LsqFilter(
  const int filter_shift,
  const int64_t value_active,
  const int64_t value_hidden)
  : m_val1(value_hidden),
    m_val2(value_active),
    m_filter_shift(filter_shift)
{
  assert(m_filter_shift >=  0 && "A bitshift should not be done with negative values");
  assert(m_filter_shift <= 63 && "An int can maximally be shifted 63 bits to the right");
}

int64_t LsqFilter::Estimate(const int64_t measurement)
{
  m_val1 += ((measurement  - m_val1) >> m_filter_shift);
  m_val2 += ((m_val1 - m_val2) >> m_filter_shift);
  return m_val2;
}
