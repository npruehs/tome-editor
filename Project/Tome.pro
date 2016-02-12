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
    ../Source/Tome/Fields/Model/fielddefinitionset.cpp \
    ../Source/Tome/Fields/Controller/fielddefinitionsetserializer.cpp \
    ../Source/Tome/Help/View/aboutwindow.cpp \
    ../Source/Tome/Projects/View/newprojectwindow.cpp \
    ../Source/Tome/Projects/Controller/projectserializer.cpp \
    ../Source/Tome/Projects/Model/project.cpp \
    ../Source/Tome/Records/Model/record.cpp \
    ../Source/Tome/Records/Model/recordset.cpp \
    ../Source/Tome/Records/Controller/recordsetserializer.cpp \
    ../Source/Tome/Fields/Model/fielddefinition.cpp \
    ../Source/Tome/Values/valueconverter.cpp \
    ../Source/Tome/IO/xmlreader.cpp \
    ../Source/Tome/Fields/View/fielddefinitionwindow.cpp \
    ../Source/Tome/Fields/View/fielddefinitionswindow.cpp \
    ../Source/Tome/Records/View/recordwindow.cpp \
    ../Source/Tome/Fields/View/fieldvaluewindow.cpp \
    ../Source/Tome/Fields/View/fieldvaluewidget.cpp \
    ../Source/Tome/Export/Model/recordexporttemplate.cpp \
    ../Source/Tome/Components/View/componentswindow.cpp \
    ../Source/Tome/Components/View/componentwindow.cpp \
    ../Source/Tome/Types/View/customtypeswindow.cpp \
    ../Source/Tome/Types/Model/customtype.cpp \
    ../Source/Tome/Types/View/enumerationwindow.cpp \
    ../Source/Tome/Types/View/enumerationmemberwindow.cpp \
    ../Source/Tome/Types/View/listwindow.cpp \
    ../Source/Tome/Fields/View/listwidget.cpp \
    ../Source/Tome/Fields/View/listitemwindow.cpp \
    ../Source/Tome/Components/Controller/componentscontroller.cpp \
    ../Source/Tome/controller.cpp \
    ../Source/Tome/Export/Controller/exportcontroller.cpp \
    ../Source/Tome/Records/Controller/recordscontroller.cpp \
    ../Source/Tome/Fields/Controller/fielddefinitionscontroller.cpp \
    ../Source/Tome/Types/Controller/typescontroller.cpp \
    ../Source/Tome/Settings/Controller/settingscontroller.cpp

HEADERS  += ../Source/Tome/mainwindow.h \
    ../Source/Tome/Types/Model/builtintype.h \
    ../Source/Tome/Fields/Model/fielddefinition.h \
    ../Source/Tome/Fields/Model/fielddefinitionset.h \
    ../Source/Tome/Fields/Controller/fielddefinitionsetserializer.h \
    ../Source/Tome/Help/View/aboutwindow.h \
    ../Source/Tome/Projects/View/newprojectwindow.h \
    ../Source/Tome/Projects/Controller/projectserializer.h \
    ../Source/Tome/Projects/Model/project.h \
    ../Source/Tome/Records/Model/record.h \
    ../Source/Tome/Records/Model/recordset.h \
    ../Source/Tome/Records/Controller/recordsetserializer.h \
    ../Source/Tome/Values/valueconverter.h \
    ../Source/Tome/Util/pathutils.h \
    ../Source/Tome/IO/xmlreader.h \
    ../Source/Tome/Fields/View/fielddefinitionwindow.h \
    ../Source/Tome/Fields/View/fielddefinitionswindow.h \
    ../Source/Tome/Records/View/recordwindow.h \
    ../Source/Tome/Fields/View/fieldvaluewindow.h \
    ../Source/Tome/Fields/View/fieldvaluewidget.h \
    ../Source/Tome/Export/Model/recordexporttemplate.h \
    ../Source/Tome/Components/View/componentswindow.h \
    ../Source/Tome/Components/View/componentwindow.h \
    ../Source/Tome/Types/View/customtypeswindow.h \
    ../Source/Tome/Types/Model/customtype.h \
    ../Source/Tome/Types/View/enumerationmemberwindow.h \
    ../Source/Tome/Types/View/enumerationwindow.h \
    ../Source/Tome/Types/View/listwindow.h \
    ../Source/Tome/Fields/View/listwidget.h \
    ../Source/Tome/Fields/View/listitemwindow.h \
    ../Source/Tome/Util/vectorutils.h \
    ../Source/Tome/Components/Controller/componentscontroller.h \
    ../Source/Tome/Components/Model/component.h \
    ../Source/Tome/controller.h \
    ../Source/Tome/Components/Model/componentlist.h \
    ../Source/Tome/Export/Controller/exportcontroller.h \
    ../Source/Tome/Records/Controller/recordscontroller.h \
    ../Source/Tome/Records/Model/recordlist.h \
    ../Source/Tome/Records/Model/recordsetlist.h \
    ../Source/Tome/Fields/Controller/fielddefinitionscontroller.h \
    ../Source/Tome/Fields/Model/fielddefinitionsetlist.h \
    ../Source/Tome/Fields/Model/fielddefinitionlist.h \
    ../Source/Tome/Types/Controller/typescontroller.h \
    ../Source/Tome/Types/Model/customtypelist.h \
    ../Source/Tome/Settings/Controller/settingscontroller.h

FORMS    += ../Source/Tome/mainwindow.ui \
    ../Source/Tome/Help/View/aboutwindow.ui \
    ../Source/Tome/Projects/View/newprojectwindow.ui \
    ../Source/Tome/Fields/View/fielddefinitionwindow.ui \
    ../Source/Tome/Fields/View/fielddefinitionswindow.ui \
    ../Source/Tome/Records/View/recordwindow.ui \
    ../Source/Tome/Fields/View/fieldvaluewindow.ui \
    ../Source/Tome/Components/View/componentswindow.ui \
    ../Source/Tome/Components/View/componentwindow.ui \
    ../Source/Tome/Types/View/customtypeswindow.ui \
    ../Source/Tome/Types/View/enumerationwindow.ui \
    ../Source/Tome/Types/View/enumerationmemberwindow.ui \
    ../Source/Tome/Types/View/listwindow.ui \
    ../Source/Tome/Fields/View/listitemwindow.ui

RESOURCES += \
    tome.qrc
