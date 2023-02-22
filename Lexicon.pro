# QMake variables
VERSION_MAJOR = 1
VERSION_MINOR = 2
VERSION_BUILD = 0
VERSION_STR = '\\"$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}\\"'
APP_NAME = Lexicon # necessary because TARGET variable doesn't like string version
APP_NAME_STR = '\\"$${APP_NAME}\\"'

QMAKE_LIBFLAGS += /VERBOSE

# C++ variables
DEFINES += "VERSION_STR=$$VERSION_STR"
DEFINES += "APP_NAME_STR=$$APP_NAME_STR"

TEMPLATE = app

VERSION = $${VERSION_STR}
TARGET = $${APP_NAME}
DESTDIR = $${_PRO_FILE_PWD_}/install

CONFIG += c++20 app_bundle sdk_no_version_check
CONFIG += release

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$_PRO_FILE_PWD_/source

SOURCES += \
    source/about.cpp \
    source/entrydisplay.cpp \
    source/entryedit.cpp \
    source/listedit.cpp \
    source/listio.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/settings.cpp

HEADERS += \
    source/about.h \
    source/entrydisplay.h \
    source/entryedit.h \
    source/entrymodel.h \
    source/listedit.h \
    source/listio.h \
    source/mainwindow.h \
    source/settings.h

FORMS += \
    source/about.ui \
    source/entrydisplay.ui \
    source/entryedit.ui \
    source/listedit.ui \
    source/mainwindow.ui \
    source/settings.ui

ICON = resources/icon.icns

RESOURCES += resources/resources.qrc
