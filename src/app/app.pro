include(../../DigitalWorkbench.pri)

TEMPLATE = app
TARGET = $$APP_TARGET
DESTDIR = $$APP_PATH
VERSION = $$APP_VERSION

QT += core

#equals(APP_TYPE, "console") {
#    QT -= gui
#    CONFIG += console
#}

equals(APP_TYPE, "gui") {
    QT += gui widgets
}


#message("config: " $$CONFIG)
#message("qt: " $$QT)

HEADERS +=
SOURCES += main.cpp

DISTFILES += \ #qtcreator.rc \
#    Info.plist \
    $$PWD/app_version.h.in

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in

LIBS *= -l$$qtLibraryName(ExtensionSystem) -l$$qtLibraryName(Aggregation) -l$$qtLibraryName(Utils)

