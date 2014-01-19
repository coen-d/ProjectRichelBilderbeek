include(../../DesktopApplication.pri)
include(../../Libraries/Boost.pri)

#General, console
include(../../Classes/CppAbout/CppAbout.pri)
include(../../Classes/CppFileIo/CppFileIo.pri)
include(../../Classes/CppHelp/CppHelp.pri)
include(../../Classes/CppMenuDialog/CppMenuDialog.pri)
include(../../Classes/CppRichelBilderbeekProgram/CppRichelBilderbeekProgram.pri)
include(../../Classes/CppTrace/CppTrace.pri)

#General, desktop
include(../../Classes/CppQtAboutDialog/CppQtAboutDialog.pri)
include(../../Classes/CppQtHideAndShowDialog/CppQtHideAndShowDialog.pri)

#Specific, console
include(../../Classes/CppCanvas/CppCanvas.pri)
include(../../Classes/CppDial/CppDial.pri)
include(../../Classes/CppDialWidget/CppDialWidget.pri)
include(../../Classes/CppDotMatrix/CppDotMatrix.pri)
include(../../Classes/CppRectangle/CppRectangle.pri)
include(../../Classes/CppTextCanvas/CppTextCanvas.pri)
include(../../Classes/CppWidget/CppWidget.pri)

#Specific, desktop
include(../../Classes/CppQtDialWidget/CppQtDialWidget.pri)
include(../../Classes/CppRainbow/CppRainbow.pri)
include(../../Tools/ToolTestDial/ToolTestDialDesktop.pri)

SOURCES += qtmain.cpp
