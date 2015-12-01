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
    ../Source/Tome/Fields/intfielddefinition.cpp \
    ../Source/Tome/Fields/fielddefinitionset.cpp \
    ../Source/Tome/Fields/fielddefinitionsetserializer.cpp \
    ../Source/Tome/Projects/projectserializer.cpp \
    ../Source/Tome/project.cpp

HEADERS  += ../Source/Tome/mainwindow.h \
    ../Source/Tome/aboutwindow.h \
    ../Source/Tome/newprojectwindow.h \
    ../Source/Tome/Fields/fieldtype.h \
    ../Source/Tome/Fields/fielddefinition.h \
    ../Source/Tome/Fields/intfielddefinition.h \
    ../Source/Tome/Fields/fielddefinitionset.h \
    ../Source/Tome/Fields/fielddefinitionsetserializer.h \
    ../Source/Tome/Projects/projectserializer.h \
    ../Source/Tome/project.h

FORMS    += ../Source/Tome/mainwindow.ui \
    ../Source/Tome/aboutwindow.ui \
    ../Source/Tome/newprojectwindow.ui
