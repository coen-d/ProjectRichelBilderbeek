//---------------------------------------------------------------------------
/*
GameConnectThree, connect-three game
Copyright (C) 2010-2013 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/GameConnectThree.htm
//---------------------------------------------------------------------------
//#include own header file as first substantive line of code, from:
// * John Lakos. Large-Scale C++ Software Design. 1996. ISBN: 0-201-63362-0. Section 3.2, page 110
#include "qtconnectthreeresources.h"

#include <cassert>

QtConnectThreeResources::QtConnectThreeResources()
  : ConnectThreeResources(
    {
      "ConnectThreeComputer1.png",
      "ConnectThreeComputer2.png",
      "ConnectThreeComputer3.png"
    },
    "ConnectThreeComputerGrey.png",
    "GameConnectThree.css",
    "ConnectThreeEmpty.png",
    "ConnectThreeIcon.png",
    {
      "ConnectThreeGood1.png",
      "ConnectThreeGood2.png"
    },
    "ConnectThreeWrong.png",
    {
      "ConnectThreePlayer1.png",
      "ConnectThreePlayer2.png",
      "ConnectThreePlayer3.png"
    },
    {
      "ConnectThreePlayer1Grey.png",
      "ConnectThreePlayer2Grey.png",
      "ConnectThreePlayer3Grey.png"
    },
    "Quit",
    "Winner"
  )
{

}
