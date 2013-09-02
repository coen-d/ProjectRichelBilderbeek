#ifndef KALMANFILTERERMENUDIALOG_H
#define KALMANFILTERERMENUDIALOG_H

#include <string>
#include <vector>

#include "about.h"

///GUI independent KalmanFilterer menu dialog
struct KalmanFiltererMenuDialog
{
  static const About GetAbout();
  static const std::string GetVersion();
  static const std::vector<std::string> GetVersionHistory();
};

#endif // KALMANFILTERERMENUDIALOG_H
