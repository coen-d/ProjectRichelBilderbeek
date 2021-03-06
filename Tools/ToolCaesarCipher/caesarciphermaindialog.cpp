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
#include "caesarciphermaindialog.h"

#include "trace.h"

ribi::CaesarCipherMainDialog::CaesarCipherMainDialog(const int key) noexcept
  : m_caesarcipher(new CaesarCipher(key)),
    m_encrypted_text{},
    m_key(key),
    m_plain_text{}
{
  #ifndef NDEBUG
  Test();
  #endif
}

void ribi::CaesarCipherMainDialog::Deencrypt() noexcept
{
  m_plain_text = m_caesarcipher->Deencrypt(m_encrypted_text);
}

void ribi::CaesarCipherMainDialog::Encrypt() noexcept
{
  m_encrypted_text = m_caesarcipher->Encrypt(m_plain_text);
}

void ribi::CaesarCipherMainDialog::SetEncryptedText(const std::string& s) noexcept
{
  m_encrypted_text = s;
}

void ribi::CaesarCipherMainDialog::SetKey(const int i) noexcept
{
  m_caesarcipher.reset(new CaesarCipher(i));
}

void ribi::CaesarCipherMainDialog::SetPlainText(const std::string& s) noexcept
{
  m_plain_text = s;
}

#ifndef NDEBUG
void ribi::CaesarCipherMainDialog::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::CaesarCipherMainDialog::Test");
  {
    CaesarCipherMainDialog d;
    const std::string s = "HELLOWORLD";
    d.SetKey(123);
    d.SetPlainText(s);
    d.Encrypt();
    const std::string encrypted = d.GetEncryptedText();
    d.SetEncryptedText(encrypted);
    d.Deencrypt();
    const std::string deencrypted = d.GetPlainText();
    assert(s == deencrypted);
  }
  TRACE("Finished ribi::CaesarCipherMainDialog::Test successfully");
}
#endif
