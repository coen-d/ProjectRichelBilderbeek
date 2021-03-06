//---------------------------------------------------------------------------
/*
  Super Tron, a simple game
  Copyright (C) 2008  Rich�l Bilderbeek

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
// From http://www.richelbilderbeek.nl
//---------------------------------------------------------------------------
#ifndef UnitSuperTronPlayerH
#define UnitSuperTronPlayerH
//---------------------------------------------------------------------------
#include <Graphics.hpp> //For TColor
//#include <windef.h>
typedef unsigned short      WORD;
//---------------------------------------------------------------------------
struct SuperTronPlayer
{
  SuperTronPlayer();
  double x;
  double y;
  double angle;
  int nWins;
  int nLose;
  Graphics::TColor color;
  WORD keyLeft;
  WORD keyRight;
  static double deltaAngle;
  static const double speed;
};

#endif
