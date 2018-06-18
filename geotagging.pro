#-------------------------------------------------
#
# Project created by QtCreator 2010-10-07T09:18:25
#
#-------------------------------------------------

QT       += core gui \
        network \
        webkit \
        widgets \
        webkitwidgets

TARGET = geotagging
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scrollarea.cpp \
    imageview.cpp \
    imageinfo.cpp \
    gpsroute.cpp \
    gpspoint.cpp \
    gpspanel.cpp \
    picturesscrollarea.cpp \
    synchdialog.cpp \
    mapwidget.cpp \
    picturesframe.cpp \
    keyeventhandler.cpp \
    changedatetime.cpp \
    datetimeedit.cpp \
    gpsrouteslist.cpp \
    synchresultdialog.cpp \
    exifreaderwriter.cpp \
    synchresultitem.cpp \
    datetimedialog.cpp \
    formathandler.cpp \
    imagewidgetslist.cpp \
    gpssegment.cpp \
    exifinfo.cpp \
    importfileprocessor.cpp \
    imagedata.cpp

HEADERS  += mainwindow.h \
    scrollarea.h \
    imageview.h \
    imageinfo.h \
    gpsroute.h \
    gpspoint.h \
    gpspanel.h \
    picturesscrollarea.h \
    synchdialog.h \
    mapwidget.h \
    picturesframe.h \
    keyeventhandler.h \
    changedatetime.h \
    datetimeedit.h \
    gpsrouteslist.h \
    synchresultdialog.h \
    exifreaderwriter.h \
    synchresultitem.h \
    datetimedialog.h \
    formathandler.h \
    imagewidgetslist.h \
    gpssegment.h \
    exifinfo.h \
    importfileprocessor.h \
    imagedata.h

FORMS    += mainwindow.ui \
    imageinfo.ui \
    synchdialog.ui \
    changedatetime.ui

TRANSLATIONS = translation/geotagging_cs_CZ.ts

CONFIG += static

unix: !andorid: {
    CONFIG += link_pkgconfig
    PKGCONFIG += exiv2
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += target
    target.path = $$BINDIR

    icons.files = ./icons/geotagging.png
    icons.path = $$PREFIX/share/icons/hicolor/64x64/apps/
    icons.CONFIG += no_check_exist
    INSTALLS += icons

    desktop.files = geotagging.desktop
    desktop.path = $$PREFIX/share/applications
    desktop.CONFIG += no_check_exist
    INSTALLS += desktop



}
win32 {
  #  LIBS += -L"C:/Qt/2010.05/mingw/msys/1.0/local/lib" -lexiv2 -lexpat -lz
    LIBS += -L"../Geotagging/libs" -lexiv2 -lexpat -lz
}

RESOURCES += resources.qrc

