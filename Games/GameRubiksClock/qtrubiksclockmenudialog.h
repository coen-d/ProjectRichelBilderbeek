//---------------------------------------------------------------------------
/*
RubiksClock. Rubik's Clock game.
Copyright (C) 2007-2012  Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/GameRubiksClock.htm
//---------------------------------------------------------------------------
#ifndef QTRUBIKSCLOCKMENUDIALOG_H
#define QTRUBIKSCLOCKMENUDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtRubiksClockMenuDialog;
}

namespace ribi {

class QtRubiksClockMenuDialog : public QtHideAndShowDialog
{
  Q_OBJECT

public:
  explicit QtRubiksClockMenuDialog(QWidget *parent = 0);
  QtRubiksClockMenuDialog(const QtRubiksClockMenuDialog&) = delete;
  QtRubiksClockMenuDialog& operator=(const QtRubiksClockMenuDialog&) = delete;
  ~QtRubiksClockMenuDialog();

protected:
  void keyPressEvent(QKeyEvent *);

private:
  Ui::QtRubiksClockMenuDialog *ui;

private slots:
  void on_button_about_clicked();
  void on_button_quit_clicked();
  void on_button_start_clicked();
};

} //~namespace ribi

#endif // QTRUBIKSCLOCKMENUDIALOG_H
