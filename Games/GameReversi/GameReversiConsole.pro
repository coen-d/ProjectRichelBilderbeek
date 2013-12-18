include(../../DesktopApplication.pri)

#Libs
include(../../Libraries/Boost.pri)
include(../../Libraries/Fparser.pri)

#Console
include(../../Classes/CppAbout/CppAbout.pri)
include(../../Classes/CppFileIo/CppFileIo.pri)
include(../../Classes/CppHelp/CppHelp.pri)
include(../../Classes/CppMenuDialog/CppMenuDialog.pri)
include(../../Classes/CppRichelBilderbeekProgram/CppRichelBilderbeekProgram.pri)
include(../../Classes/CppTrace/CppTrace.pri)

#Desktop
include(../../Classes/CppQtAboutDialog/CppQtAboutDialog.pri)
include(../../Classes/CppQtHideAndShowDialog/CppQtHideAndShowDialog.pri)

#Specific for this application
#Console
include(../../Classes/CppReversi/CppReversi.pri)
include(../../Game/GameReversi/GameReversiConsole.pri)

SOURCES +=  \
  main.cpp