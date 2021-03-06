//---------------------------------------------------------------------------
/*
CaesarCipher, Caesar cipher tool
Copyright (C) 2014-2014 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/ToolCaesarCipher.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtcaesarciphermaindialog.h"

#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "caesarcipher.h"
#include "loopreader.h"
#include "qtaboutdialog.h"
#include "trace.h"
#include "ui_qtcaesarciphermaindialog.h"
#include "caesarciphermaindialog.h"
#pragma GCC diagnostic pop

ribi::QtCaesarCipherMainDialog::QtCaesarCipherMainDialog(QWidget *parent) noexcept
 :  QtHideAndShowDialog(parent),
    ui(new Ui::QtCaesarCipherMainDialog),
    m_dialog(new CaesarCipherMainDialog)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
}

ribi::QtCaesarCipherMainDialog::~QtCaesarCipherMainDialog() noexcept
{
  delete ui;
  delete m_dialog;
}

void ribi::QtCaesarCipherMainDialog::on_button_encrypt_clicked() noexcept
{
  const std::string text = ui->edit_plaintext->text().toStdString();
  for (auto c:text) { if (c < 'A' || c > 'Z') return; }

  const int key = ui->box_key->value();
  assert(key >= 0);

  m_dialog->SetPlainText(text);
  m_dialog->SetKey(key);
  m_dialog->Encrypt();
  const std::string result = m_dialog->GetEncryptedText();
  if (ui->edit_encrypted_text->text().toStdString() != result)
  {
    ui->edit_encrypted_text->setText(result.c_str());
  }
}

void ribi::QtCaesarCipherMainDialog::on_button_deencrypt_clicked() noexcept
{
  const std::string text = ui->edit_encrypted_text->text().toStdString();
  for (auto c:text) { if (c < 'A' || c > 'Z') return; }

  const int key = ui->box_key->value();
  m_dialog->SetEncryptedText(text);
  m_dialog->SetKey(key);
  m_dialog->Deencrypt();

  const std::string result = m_dialog->GetPlainText();
  if (ui->edit_plaintext->text().toStdString() != result)
  {
    ui->edit_plaintext->setText(result.c_str());
  }
}

#ifndef NDEBUG
void ribi::QtCaesarCipherMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::QtCaesarCipherMainDialog::Test");
  TRACE("Finished ribi::QtCaesarCipherMainDialog::Test successfully");
}
#endif

void ribi::QtCaesarCipherMainDialog::on_edit_plaintext_textChanged(const QString&)
{
  on_button_encrypt_clicked();
}

void ribi::QtCaesarCipherMainDialog::on_edit_encrypted_text_textChanged(const QString&)
{
  on_button_deencrypt_clicked();
}
