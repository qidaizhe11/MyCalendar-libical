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
    src/diarywindow.cpp \
    src/calendarmodel.cpp \
    src/context.cpp \
    src/athlete.cpp

HEADERS  += \
    src/diarywindow.h \
    src/calendarmodel.h \
    src/context.h \
    src/athlete.h

OTHER_FILES += \
    README.md

RESOURCES += \
    src/mycalendar.qrc
