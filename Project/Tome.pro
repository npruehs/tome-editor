#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T20:25:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tome
TEMPLATE = app

Debug:DESTDIR = ../Bin/debug
Debug:OBJECTS_DIR = ../Obj/debug
Debug:MOC_DIR = ../Obj/debug/.moc
Debug:RCC_DIR = ../Obj/debug/.rcc
Debug:UI_DIR = ../Obj/debug/.ui

SOURCES += ../Source/Tome/main.cpp \
    ../Source/Tome/mainwindow.cpp \
    ../Source/Tome/aboutwindow.cpp \
    ../Source/Tome/newprojectwindow.cpp \
    ../Source/Tome/Fields/fielddefinitionfile.cpp \
    ../Source/Tome/tomeproject.cpp \
    ../Source/Tome/Fields/intfielddefinition.cpp \
    ../Source/Tome/Projects/tomeprojectserializer.cpp \
    ../Source/Tome/Fields/fielddefinitionfileserializer.cpp

HEADERS  += ../Source/Tome/mainwindow.h \
    ../Source/Tome/aboutwindow.h \
    ../Source/Tome/newprojectwindow.h \
    ../Source/Tome/Fields/fieldtype.h \
    ../Source/Tome/Fields/fielddefinition.h \
    ../Source/Tome/Fields/fielddefinitionfile.h \
    ../Source/Tome/tomeproject.h \
    ../Source/Tome/Fields/intfielddefinition.h \
    ../Source/Tome/Projects/tomeprojectserializer.h \
    ../Source/Tome/Fields/fielddefinitionfileserializer.h

FORMS    += ../Source/Tome/mainwindow.ui \
    ../Source/Tome/aboutwindow.ui \
    ../Source/Tome/newprojectwindow.ui
