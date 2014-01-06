//---------------------------------------------------------------------------
/*
NewickCpp98, C++98 Newick functions
Copyright (C) 2010-2014 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppNewick.htm
//---------------------------------------------------------------------------
#ifndef NEWICKCPP98_H
#define NEWICKCPP98_H

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/tuple/tuple.hpp>
#pragma GCC diagnostic pop

namespace ribi {

namespace NewickCpp98 {

//Functions that do not use the C++0x standard
const std::vector<std::string> CreateValidTrinaryNewicks();
std::vector<boost::tuple<std::string,double,double> > GetKnownProbabilities();
const std::vector<std::pair<std::vector<int>,int> > GetSimplerNewicksFrequencyPairs(const std::vector<int>& n);
const std::vector<std::vector<int> > GetRootBranches(const std::vector<int>& n);
#ifndef NDEBUG
void Test();
#endif

} //~namespace NewickCpp98

} //~namespace ribi

#endif // NEWICKCPP98_H
