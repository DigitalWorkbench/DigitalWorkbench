!isEmpty(DIGITALWORKBENCH_PRI_INCLUDED):error("DigitalWorkbench.pri already included")
DIGITALWORKBENCH_PRI_INCLUDED = 1

## Application parameters
APP_VERSION = 0.1.0
APP_TARGET = "dw"
#DEST_TREE =   #default to project root directory
APP_TYPE = gui  # this must be set to console or gui

##Include for the QExtensionSystem

QES_LIBRARY_PATH = $$PWD/src/3rdparty/qextensionsystem
isEmpty(QES_LIBRARY_PATH) {
    #error("must define QES_LIBRARY_PATH with the path of QExtension System Library")
    message("must define QES_LIBRARY_PATH with the path of QExtension System Library")
} else {
#    include($$QES_LIBRARY_PATH/qeslibrary.pri)
#    include($$QES_LIBRARY_PATH/qextensionsystem.pri)
}

DEFINES += APP_VERSION_STR=\"$${APP_VERSION}\"
#equals(APP_TYPE, "console"):
#equals(APP_TYPE, "gui"): DEFINES += GUI

#define type of app
!isEmpty(APP_TYPE) {
    CONFIG += console
    QT -= gui
    DEFINES += CONSOLE
}

!isEmpty(PREFIX):DEST_TREE=$$PREFIX

BASE_TREE = $$PWD
SOURCE_TREE = $$PWD
3RDPARTY_SRC = $$SOURCE_TREE/src/3rdparty
isEmpty(DEST_TREE): DEST_TREE = $$BASE_TREE

BUILD_TREE = $$DEST_TREE/build
TMP_PATH = $$DEST_TREE/build/tmp
APP_PATH = $$BUILD_TREE/bin
LIB_PATH = $$BUILD_TREE/lib

INCLUDEPATH += $$QES_LIBRARY_PATH/src/libs

##PLUGIN_PATH = $$BUILD_TREE/lib/plugins

CONFIG(debug, debug|release):OBJECTS_DIR = $${TMP_PATH}/.obj/debug-shared/$${TARGET}
CONFIG(release, debug|release):OBJECTS_DIR = $${TMP_PATH}/.obj/release-shared/$${TARGET}

CONFIG(debug, debug|release):MOC_DIR = $${TMP_PATH}/.moc/debug-shared/$${TARGET}
CONFIG(release, debug|release):MOC_DIR = $${TMP_PATH}/.moc/release-shared/$${TARGET}

RCC_DIR = $${TMP_PATH}/.rcc
UI_DIR = $${TMP_PATH}/.uic

LIBS += -L$$LIB_PATH -L$$LIB_PATH/qes
#-L$$LIB_PATH/qtcreator

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}
