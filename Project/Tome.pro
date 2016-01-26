#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T20:25:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tome
TEMPLATE = app

# Expose application version in Windows property window and in application code.
# http://www.openguru.com/2009/11/qt-best-way-to-set-application-version.html
VERSION = 0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

Debug:DESTDIR = ../Bin/debug
Debug:OBJECTS_DIR = ../Obj/debug
Debug:MOC_DIR = ../Obj/debug/.moc
Debug:RCC_DIR = ../Obj/debug/.rcc
Debug:UI_DIR = ../Obj/debug/.ui

Release:DESTDIR = ../Bin/release
Release:OBJECTS_DIR = ../Obj/release
Release:MOC_DIR = ../Obj/release/.moc
Release:RCC_DIR = ../Obj/release/.rcc
Release:UI_DIR = ../Obj/release/.ui

SOURCES += ../Source/Tome/main.cpp \
    ../Source/Tome/mainwindow.cpp \
    ../Source/Tome/Fields/fielddefinitionset.cpp \
    ../Source/Tome/Fields/fielddefinitionsetserializer.cpp \
    ../Source/Tome/Help/aboutwindow.cpp \
    ../Source/Tome/Projects/newprojectwindow.cpp \
    ../Source/Tome/Projects/projectserializer.cpp \
    ../Source/Tome/Projects/project.cpp \
    ../Source/Tome/Records/record.cpp \
    ../Source/Tome/Records/recordset.cpp \
    ../Source/Tome/Records/recordsetserializer.cpp \
    ../Source/Tome/Fields/fielddefinition.cpp \
    ../Source/Tome/Values/valueconverter.cpp \
    ../Source/Tome/IO/xmlreader.cpp \
    ../Source/Tome/Fields/fielddefinitionstablemodel.cpp \
    ../Source/Tome/Fields/fielddefinitionwindow.cpp \
    ../Source/Tome/Fields/fielddefinitionswindow.cpp \
    ../Source/Tome/Records/recordsitemmodel.cpp \
    ../Source/Tome/Records/recordwindow.cpp \
    ../Source/Tome/Records/recordtablemodel.cpp \
    ../Source/Tome/Fields/fieldvaluewindow.cpp \
    ../Source/Tome/Fields/fieldvaluewidget.cpp \
    ../Source/Tome/Export/recordexporttemplate.cpp \
    ../Source/Tome/Export/recordexporter.cpp \
    ../Source/Tome/Components/componentswindow.cpp \
    ../Source/Tome/Components/componentsitemmodel.cpp \
    ../Source/Tome/Components/componentwindow.cpp \
    ../Source/Tome/Types/customtypeswindow.cpp \
    ../Source/Tome/Types/customtype.cpp \
    ../Source/Tome/Types/customtypesitemmodel.cpp \
    ../Source/Tome/Settings/tomesettings.cpp \
    ../Source/Tome/Types/enumerationwindow.cpp \
    ../Source/Tome/Types/enumerationmemberwindow.cpp \
    ../Source/Tome/Types/listwindow.cpp

HEADERS  += ../Source/Tome/mainwindow.h \
    ../Source/Tome/Types/builtintype.h \
    ../Source/Tome/Fields/fielddefinition.h \
    ../Source/Tome/Fields/fielddefinitionset.h \
    ../Source/Tome/Fields/fielddefinitionsetserializer.h \
    ../Source/Tome/Help/aboutwindow.h \
    ../Source/Tome/Projects/newprojectwindow.h \
    ../Source/Tome/Projects/projectserializer.h \
    ../Source/Tome/Projects/project.h \
    ../Source/Tome/Records/record.h \
    ../Source/Tome/Records/recordset.h \
    ../Source/Tome/Records/recordsetserializer.h \
    ../Source/Tome/Values/valueconverter.h \
    ../Source/Tome/Util/pathutils.h \
    ../Source/Tome/IO/xmlreader.h \
    ../Source/Tome/Fields/fielddefinitionstablemodel.h \
    ../Source/Tome/Fields/fielddefinitionwindow.h \
    ../Source/Tome/Fields/fielddefinitionswindow.h \
    ../Source/Tome/Records/recordsitemmodel.h \
    ../Source/Tome/Records/recordwindow.h \
    ../Source/Tome/Records/recordtablemodel.h \
    ../Source/Tome/Fields/fieldvaluewindow.h \
    ../Source/Tome/Fields/fieldvaluewidget.h \
    ../Source/Tome/Export/recordexporttemplate.h \
    ../Source/Tome/Export/recordexporter.h \
    ../Source/Tome/Components/componentswindow.h \
    ../Source/Tome/Components/componentsitemmodel.h \
    ../Source/Tome/Components/componentwindow.h \
    ../Source/Tome/Types/customtypeswindow.h \
    ../Source/Tome/Types/customtype.h \
    ../Source/Tome/Types/customtypesitemmodel.h \
    ../Source/Tome/Settings/tomesettings.h \
    ../Source/Tome/Types/enumerationmemberwindow.h \
    ../Source/Tome/Types/enumerationwindow.h \
    ../Source/Tome/Types/listwindow.h

FORMS    += ../Source/Tome/mainwindow.ui \
    ../Source/Tome/Help/aboutwindow.ui \
    ../Source/Tome/Projects/newprojectwindow.ui \
    ../Source/Tome/Fields/fielddefinitionwindow.ui \
    ../Source/Tome/Fields/fielddefinitionswindow.ui \
    ../Source/Tome/Records/recordwindow.ui \
    ../Source/Tome/Fields/fieldvaluewindow.ui \
    ../Source/Tome/Components/componentswindow.ui \
    ../Source/Tome/Components/componentwindow.ui \
    ../Source/Tome/Types/customtypeswindow.ui \
    ../Source/Tome/Types/enumerationwindow.ui \
    ../Source/Tome/Types/enumerationmemberwindow.ui \
    ../Source/Tome/Types/listwindow.ui

RESOURCES += \
    tome.qrc
