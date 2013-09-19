//---------------------------------------------------------------------------
/*
QtCreatorProFile, class to parse Qt Project files
Copyright (C) 2010-2013 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtCreatorProFile.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtcreatorprofile.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/function.hpp>

#include <QDir>
#include <QFile>

#include "trace.h"

#pragma GCC diagnostic pop

ribi::QtCreatorProFile::QtCreatorProFile(const std::string& filename)
  :
    m_config{},
    m_defines{},
    m_forms{},
    m_headers{},
    m_includepath{},
    m_libs{},
    m_other_files{},
    m_pro_filename{filename},
    m_qmake_cxxflags{},
    m_qt{},
    m_resources{},
    m_target{},
    m_template{},
    m_sources{}
{
  #ifndef NDEBUG
  Test();
  #endif

  #ifndef NDEBUG
  if (!IsRegularFile(filename))
  {
    TRACE(filename);
    TRACE("BREAK");
  }
  #endif
  assert(IsRegularFile(filename));

  const std::vector<std::string> v = FileToVector(filename);
  Parse(v);
}

const std::vector<std::string> ribi::QtCreatorProFile::FileToVector(const std::string& filename)
{
  assert(IsRegularFile(filename.c_str()));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  std::string s;
  for (int i=0; !in.eof(); ++i)
  {
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

const ribi::About ribi::QtCreatorProFile::GetAbout() noexcept
{
  ribi::About a(
    "Richel Bilderbeek",
    "QtCreatorProFile",
    "class to parse Qt Project files",
    "the 19th of August 2013",
    "2010-2013",
    "http://www.richelbilderbeek.nl/CppQtCreatorProFile.htm",
    GetVersion(),
    GetVersionHistory());
  return a;
}

const std::string ribi::QtCreatorProFile::GetVersion() noexcept
{
  return "2.1";
}

const std::vector<std::string> ribi::QtCreatorProFile::GetVersionHistory() noexcept
{
  return {
    "2010-12-19: version 1.0: initial version",
    "2011-01-06: version 1.1: added GetCommonRoot and GetLibs member functions, added operator<<",
    "2011-09-11: version 1.2: fixed bug",
    "2012-02-25: version 1.3: added GetAbout member function",
    "2012-02-28: version 1.4: added support for INCLUDEPATH, FORMS, OTHER_FILES, RESOURCES and QMAKE_CXXFLAGS",
    "2012-05-30: version 1.5: added SimplifyPath",
    "2012-08-13: version 1.6: modifiers like win32 and unix are ignored, instead of yielding an error",
    "2012-12-23: version 1.7: set destructor to private, except for boost::checked_delete",
    "2012-12-23: version 1.8: renamed to QtCreatorProFile due to naming conflicts when cross-compiling",
    "2013-05-18: version 2.0: simplified architecture by removing file I/O",
    "2013-08-19: version 2.1: replaced Boost.Regex by Boost.Xpressive, removed Boost.Filesystem",
  };
}

#ifndef NDEBUG
bool ribi::QtCreatorProFile::IsRegularFile(const std::string& filename)
{
  return !QDir(filename.c_str()).exists() && QFile::exists(filename.c_str());
}
#endif

void ribi::QtCreatorProFile::Parse(const std::vector<std::string> &v)
{
  std::set<std::string> * p = nullptr; //A set to write to
  enum class Prefix { none, plus, minus };
  Prefix prefix = Prefix::none;
  for (const std::string& dirty_line: v)
  {
    const std::string line = boost::algorithm::trim_copy(dirty_line);
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    if (line[0] == '{') continue;
    if (line[0] == '}') continue;
    if (line.size() >= 7 && line.substr(0,7) == std::string("message")) continue;
    const std::vector<std::string> words = SeperateString(line,' ');
    for (const std::string& dirty_word: words)
    {
      const std::string word = boost::algorithm::trim_copy(dirty_word);
      if (word.empty()) continue;
      if (word[0] == '#') break; //Skip the rest of the line
      assert(!word.empty());
      std::string s = word; //s gets cut into bits
      const std::vector<std::string> conditional_sections
        =
        {
          "unix", "win32", "static", "debug", "release"
        };
      if (std::count(conditional_sections.begin(),conditional_sections.end(),s))
      {
        p = nullptr;
        continue;
      }
      const std::map<std::string,std::set<std::string>* > m
        =
        {
          { "CONFIG"        ,&m_config },
          { "DEFINES"       ,&m_defines },
          { "FORMS"         ,&m_forms },
          { "HEADERS"       ,&m_headers },
          { "INCLUDEPATH"   ,&m_includepath },
          { "LIBS"          ,&m_libs },
          { "OTHER_FILES"   ,&m_other_files },
          { "QMAKE_CXXFLAGS",&m_qmake_cxxflags },
          { "QT"            ,&m_qt },
          { "RESOURCES"     ,&m_resources },
          { "SOURCES"       ,&m_sources },
          { "TARGET"        ,&m_target },
          { "TEMPLATE"      ,&m_template }
        };
      for (const std::pair<std::string,std::set<std::string>* > sub_pair: m)
      {
        const std::string& sub = sub_pair.first;
        if (s.size() >= sub.size() && s.substr(0,sub.size()) == sub)
        {
          p = sub_pair.second;
          assert(p);
          s = s.substr(sub.size(),s.size() - sub.size());
          prefix = Prefix::none;
        }
      }
      //Determine prefixes
      bool has_prefixes = true;
      while (has_prefixes)
      {
        has_prefixes = false;
        if (!s.empty() && s[0] == '+')
        {
          prefix = Prefix::plus;
          s = s.substr(1,s.size() - 1);
          has_prefixes = true;
        }
        else if (!s.empty() && s[0] == '-')
        {
          prefix = Prefix::minus;
          s = s.substr(1,s.size() - 1);
          has_prefixes = true;
        }
        else if (!s.empty() && s[0] == '\\')
        {
          s = s.substr(1,s.size() - 1);
          has_prefixes = true;
        }
        else if (!s.empty() && s[0] == '=')
        {
          s = s.substr(1,s.size() - 1);
          has_prefixes = true;
        }
      }
      //Remove possible postfix
      if (!s.empty())
      {
        if (s[ s.size() - 1] == '\\') s.resize(s.size() - 1);

      }
      if (p && !s.empty())
      {
        p->insert(
          (prefix == Prefix::minus ? std::string("-") : std::string()) + s);
      }
    }
  }
}

const std::vector<std::string> ribi::QtCreatorProFile::SeperateString(
  const std::string& input,
  const char seperator)
{
  std::istringstream is(input);
  std::vector<std::string> v;
  for (
    std::string sub;
    std::getline(is, sub, seperator);
    v.push_back(sub))
  {
    //Empty for loop
  }
  return v;
}

#ifndef NDEBUG
void ribi::QtCreatorProFile::Test()
{
  //Test exactly once
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting QtCreatorProFile::Test");
 // TRACE("Test QtCreatorProFile::QtCreatorProFile");
  {
    const std::string mypath = "tmp.txt";
    {
      std::ofstream f(mypath);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       += core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lwt -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFile\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += main.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetConfig().size() == 2);
    assert(p.GetConfig().count("console"));
    assert(p.GetConfig().count("-app_bundle"));
    assert(p.GetHeaders().size() == 1);
    assert(p.GetHeaders().count("profile.h"));
    assert(p.GetLibs().size() == 2);
    assert(p.GetLibs().count("-lwt"));
    assert(p.GetLibs().count("-lboost_regex"));
    assert(p.GetQtCreatorProFilename() == mypath);
    assert(p.GetQt().size() == 2);
    assert(p.GetQt().count("core"));
    assert(p.GetQt().count("-gui"));
    assert(p.GetSources().size() == 2);
    assert(p.GetSources().count("main.cpp"));
    assert(p.GetSources().count("profile.cpp"));
    assert(p.GetTarget().count("ToolTestQtCreatorProFile") == 1);
    assert(p.GetTemplate().size() == 1);
    assert(p.GetTemplate().count("app"));
    //TRACE("Test QtCreatorProFile::operator<<");
    {
      //std::cout << p << '\n';
      std::stringstream ss;
      ss << p << '\n';
    }
    //TRACE("Test QtCreatorProFile::operator==");
    {
      QtCreatorProFile q(mypath);
      assert(p == q);
    }
    std::remove(mypath.c_str());
  }
  //TRACE("Test QtCreatorProFile::Merge");
  {
    const std::string mypath1 = "tmp1.txt";
    const std::string mypath2 = "tmp2.txt";
    {
      std::ofstream f(mypath1);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       += core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lwt -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFileWebsite\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += wtmain.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    {
      std::ofstream f(mypath2);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       -= core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFileConsole\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += main.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const boost::shared_ptr<const QtCreatorProFile> p1(new QtCreatorProFile(mypath1));
    const boost::shared_ptr<const QtCreatorProFile> p2(new QtCreatorProFile(mypath2));
    std::remove(mypath1.c_str());
    std::remove(mypath2.c_str());
  }
  //Test conditionals
  {
    //Create a project file
    const std::string mypath = "tmp.txt";
    {
      std::ofstream f(mypath);
      f
        << "QT       += core\n"
        << "unix {\n"
        << "SOURCES += \\\n"
        << "    unix_main.cpp \\\n"
        << "}\n"
        << "win32 {\n"
        << "TARGET = TestTarget\n"
        << "SOURCES += win_main.cpp\n"
        << "}\n"
        << "SOURCES += profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetQt().size() == 1);
    assert(p.GetQt().count("core"));
    assert(p.GetSources().size() == 3);
    assert(p.GetSources().count("unix_main.cpp"));
    assert(p.GetSources().count("win_main.cpp"));
    assert(p.GetSources().count("profile.cpp"));
    assert(p.GetHeaders().size() == 1);
    assert(p.GetHeaders().count("profile.h"));

    //Test operator<<
    {
      //std::cout << p << '\n';
      std::stringstream ss;
      ss << p << '\n';
    }
    //Test operator==
    {
      QtCreatorProFile q(mypath);
      assert(p == q);
    }
    std::remove(mypath.c_str());
  }
  //IsRegularFile
  {
    {
      std::remove("tmp.txt");

      //Create a regular file
      assert(!IsRegularFile("tmp.txt"));
      {
        std::fstream f;
        f.open("tmp.txt",std::ios::out);
        f << "TEMP TEXT";
        f.close();
      }
      assert(IsRegularFile("tmp.txt"));

      std::remove("tmp.txt");

      assert(!IsRegularFile("tmp.txt"));
    }
    {
      //Create a folder
      const int mkdir_error = std::system("mkdir tmp");
      assert(!mkdir_error);
      assert(!IsRegularFile("tmp"));
      const int rmdir_error = std::system("rmdir tmp");
      assert(!rmdir_error);
    }
  }
  TRACE("Finished QtCreatorProFile::Test successfully");
}
#endif
std::ostream& ribi::operator<<(std::ostream& os, const boost::shared_ptr<QtCreatorProFile>& p)
{
  assert(p);
  os << (*p);
  return os;
}

std::ostream& ribi::operator<<(std::ostream& os, const boost::shared_ptr<const QtCreatorProFile>& p)
{
  assert(p);
  os << (*p);
  return os;
}

std::ostream& ribi::operator<<(std::ostream& os, const QtCreatorProFile& p)
{
  os << "\n";

  {
    const std::vector<
      std::pair<
        std::string, boost::function<const std::set<std::string>& (const QtCreatorProFile&)>
        >
      > v = {
        { "CONFIG", &QtCreatorProFile::GetConfig },
        { "DEFINES", &QtCreatorProFile::GetDefines },
        { "FORMS", &QtCreatorProFile::GetForms },
        { "HEADERS", &QtCreatorProFile::GetHeaders },
        { "INCLUDEPATH", &QtCreatorProFile::GetIncludepath },
        { "LIBS", &QtCreatorProFile::GetLibs },
        { "OTHER_FILES", &QtCreatorProFile::GetOtherFiles },
        { "QMAKE_CXXFLAGS", &QtCreatorProFile::GetQmakeCxxflags },
        { "QT", &QtCreatorProFile::GetQt},
        { "RESOURCES", &QtCreatorProFile::GetResources },
        { "SOURCES", &QtCreatorProFile::GetSources },
        { "TARGET", &QtCreatorProFile::GetTarget },
        { "TEMPLATE", &QtCreatorProFile::GetTemplate }
      };

    std::for_each(v.begin(),v.end(),
      [&os,&p](const std::pair<std::string, boost::function<const std::set<std::string>& (const QtCreatorProFile&)> >& pair)
      {
        const std::set<std::string>& w = pair.second(p);
        if (!w.empty())
        {
          os << pair.first << " += \\\n";
          if (w.size() > 1)
          {
            std::transform(w.begin(),--w.end(),std::ostream_iterator<std::string>(os," \\\n"),
              [](const std::string& s) { return std::string("    ") + s; } );
          }
          os << "    " + (*(--w.end())) + '\n';
          os << "\n";
        }
      }
    );
  }
  os << "\n";
  os << "#--------------------------------------------------------------------------\n";
  os << "# This file was created by:\n";
  os << "#\n";
  {
    const std::vector<std::string> v = p.GetAbout().CreateAboutText();
    std::transform(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"),
      [](const std::string& s) { return std::string("# ") + s; } );
  }
  os << "#\n";
  os << "#\n";
  os << "#\n";
  {
    const std::vector<std::string> v = p.GetAbout().CreateLicenceText();
    std::transform(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"),
      [](const std::string& s) { return std::string("# ") + s; } );
  }
  os << "#--------------------------------------------------------------------------";
  return os;
}

bool ribi::operator==(const QtCreatorProFile& lhs, const QtCreatorProFile& rhs)
{
  return
       lhs.m_config == rhs.m_config
    && lhs.m_defines == rhs.m_defines
    && lhs.m_forms == rhs.m_forms
    && lhs.m_headers == rhs.m_headers
    && lhs.m_includepath == rhs.m_includepath
    && lhs.m_libs == rhs.m_libs
    && lhs.m_other_files == rhs.m_other_files
    && lhs.m_pro_filename == rhs.m_pro_filename
    && lhs.m_qmake_cxxflags == rhs.m_qmake_cxxflags
    && lhs.m_qt == rhs.m_qt
    && lhs.m_resources == rhs.m_resources
    && lhs.m_target == rhs.m_target
    && lhs.m_template == rhs.m_template
    && lhs.m_sources == rhs.m_sources;
}
