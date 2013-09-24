#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtaboutdialog.h"
#include "qtsecretmessagecreatedialog.h"
#include "qtsecretmessageextractdialog.h"
#include "qtsecretmessagemenudialog.h"
#include "secretmessagemenudialog.h"
#include "ui_qtsecretmessagemenudialog.h"
#pragma GCC diagnostic pop

ribi::QtSecretMessageMenuDialog::QtSecretMessageMenuDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtSecretMessageMenuDialog)
{
  ui->setupUi(this);
}

ribi::QtSecretMessageMenuDialog::~QtSecretMessageMenuDialog() noexcept
{
  delete ui;
}


void ribi::QtSecretMessageMenuDialog::on_button_about_clicked()
{
  About a = SecretMessage::MenuDialog::GetAbout();
  //a.AddLibrary("QtRichelBilderbeekGalleryDialog version: " + QtRichelBilderbeekGalleryDialog::GetVersion());
  //a.AddLibrary("QtRichelBilderbeekResources version: " + RichelBilderbeek::QtResources::GetVersion());
  QtAboutDialog d(a);
  d.setStyleSheet(this->styleSheet());
  d.setWindowIcon(this->windowIcon());
  this->ShowChild(&d);
}

void ribi::QtSecretMessageMenuDialog::on_button_create_clicked()
{
  QtSecretMessageCreateDialog d;
  this->ShowChild(&d);
}

void ribi::QtSecretMessageMenuDialog::on_button_extract_clicked()
{
  QtSecretMessageExtractDialog d;
  this->ShowChild(&d);
}


void ribi::QtSecretMessageMenuDialog::on_button_quit_clicked()
{
  close();
}


