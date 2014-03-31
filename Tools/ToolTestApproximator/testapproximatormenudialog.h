#ifndef TOOLTESTAPPROXIMATORMENUDIALOG_H
#define TOOLTESTAPPROXIMATORMENUDIALOG_H

#include "menudialog.h"


namespace ribi {

struct ToolTestApproximatorMenuDialog : public MenuDialog
{
  About GetAbout() const noexcept;
  Help GetHelp() const noexcept;
  boost::shared_ptr<const Program> GetProgram() const noexcept;
  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  private:
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // TOOLTESTAPPROXIMATORMENUDIALOG_H