#-------------------------------------------------
#
# Project created by QtCreator 2013-11-02T10:15:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCalendar2
TEMPLATE = app


SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/mycalendar.cpp \
    src/diarywindow.cpp

HEADERS  += \
    src/mainwindow.h \
    src/mycalendar.h \
    src/diarywindow.h

OTHER_FILES += \
    README.md

RESOURCES += \
    src/mycalendar.qrc
