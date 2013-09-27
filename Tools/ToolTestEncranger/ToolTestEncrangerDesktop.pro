QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app
INCLUDEPATH += \
    ../../Classes/CppAbout \
    ../../Classes/CppEncranger \
    ../../Classes/CppLoopReader \
    ../../Classes/CppQtAboutDialog \
    ../../Classes/CppQtHideAndShowDialog

SOURCES += \
    ../../Classes/CppAbout/about.cpp \
    ../../Classes/CppEncranger/encranger.cpp \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.cpp \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.cpp \
    qtmain.cpp \
    qttestencrangerdialog.cpp \
    testencrangerdialog.cpp

HEADERS  += \
    ../../Classes/CppAbout/about.h \
    ../../Classes/CppEncranger/encranger.h \
    ../../Classes/CppLoopReader/loopreader.h \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.h \
    ../../Classes/CppQtHideAndShowDialog/qthideandshowdialog.h \
    qttestencrangerdialog.h \
    testencrangerdialog.h

FORMS    += \
    ../../Classes/CppQtAboutDialog/qtaboutdialog.ui \
    qttestencrangerdialog.ui

RESOURCES += \
    ToolTestEncranger.qrc

OTHER_FILES += \
    ../../Classes/CppQtHideAndShowDialog/Licence.txt

#
#
# Type of compile
#
#

CONFIG(release, debug|release) {
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++

unix {
  QMAKE_CXXFLAGS += -Werror
}

#
#
# Boost
#
#

win32 {
  INCLUDEPATH += \
    ../../Libraries/boost_1_54_0
}

