//---------------------------------------------------------------------------
/*
TicTacToe, tic-tac-toe game
Copyright (C) 2010-2011 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/GameTicTacToe.htm
//---------------------------------------------------------------------------


#include "qttictactoewinnerdialog.h"
#include "ui_qttictactoewinnerdialog.h"

ribi::QtTicTacToeWinnerDialog::QtTicTacToeWinnerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtTicTacToeWinnerDialog)
{
  ui->setupUi(this);
}

ribi::QtTicTacToeWinnerDialog::~QtTicTacToeWinnerDialog()
{
  delete ui;
}

void ribi::QtTicTacToeWinnerDialog::SetDraw()
{
  ui->label_title->setText("Draw");
  this->setWindowTitle("Draw");
  ui->label_winner->setText(" ");
}

void ribi::QtTicTacToeWinnerDialog::SetWinnerCross()
{
  ui->label_winner->setPixmap(QPixmap(":/images/X.png"));
}

void ribi::QtTicTacToeWinnerDialog::SetWinnerCircle()
{
  ui->label_winner->setPixmap(QPixmap(":/images/O.png"));
}

