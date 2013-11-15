#ifndef QTATHLETICLANDMAINDIALOG_H
#define QTATHLETICLANDMAINDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtAthleticLandMainDialog;
}

namespace ribi {
class QtAthleticLandMainDialog : public QtHideAndShowDialog
{
  Q_OBJECT

public:
  explicit QtAthleticLandMainDialog(QWidget *parent = 0);
  QtAthleticLandMainDialog(const QtAthleticLandMainDialog&) = delete;
  QtAthleticLandMainDialog& operator=(const QtAthleticLandMainDialog&) = delete;

  ~QtAthleticLandMainDialog();

private:
  Ui::QtAthleticLandMainDialog *ui;
};
} //~namespace ribi

#endif // QTATHLETICLANDMAINDIALOG_H