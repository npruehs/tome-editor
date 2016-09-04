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
VERSION = 0.4
VERSION_NAME = Chimera
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_VERSION_NAME=\\\"$$VERSION_NAME\\\"

Debug:DESTDIR = ../../Bin/debug
Debug:OBJECTS_DIR = ../../Obj/debug
Debug:MOC_DIR = ../../Obj/debug/.moc
Debug:RCC_DIR = ../../Obj/debug/.rcc
Debug:UI_DIR = ../../Obj/debug/.ui

Release:DESTDIR = ../../Bin/release
Release:OBJECTS_DIR = ../../Obj/release
Release:MOC_DIR = ../../Obj/release/.moc
Release:RCC_DIR = ../../Obj/release/.rcc
Release:UI_DIR = ../../Obj/release/.ui

SOURCES += ../Source/Tome/main.cpp \
    ../Source/Tome/Core/mainwindow.cpp \
    ../Source/Tome/Features/Fields/Model/fielddefinitionset.cpp \
    ../Source/Tome/Features/Fields/Controller/fielddefinitionsetserializer.cpp \
    ../Source/Tome/Features/Help/View/aboutwindow.cpp \
    ../Source/Tome/Features/Projects/View/newprojectwindow.cpp \
    ../Source/Tome/Features/Projects/Controller/projectserializer.cpp \
    ../Source/Tome/Features/Projects/Model/project.cpp \
    ../Source/Tome/Features/Records/Model/record.cpp \
    ../Source/Tome/Features/Records/Model/recordset.cpp \
    ../Source/Tome/Features/Records/Controller/recordsetserializer.cpp \
    ../Source/Tome/Features/Fields/Model/fielddefinition.cpp \
    ../Source/Tome/IO/xmlreader.cpp \
    ../Source/Tome/Features/Fields/View/fielddefinitionwindow.cpp \
    ../Source/Tome/Features/Fields/View/fielddefinitionswindow.cpp \
    ../Source/Tome/Features/Records/View/recordwindow.cpp \
    ../Source/Tome/Features/Fields/View/fieldvaluewindow.cpp \
    ../Source/Tome/Features/Fields/View/fieldvaluewidget.cpp \
    ../Source/Tome/Features/Export/Model/recordexporttemplate.cpp \
    ../Source/Tome/Features/Components/View/componentswindow.cpp \
    ../Source/Tome/Features/Components/View/componentwindow.cpp \
    ../Source/Tome/Features/Types/View/customtypeswindow.cpp \
    ../Source/Tome/Features/Types/Model/customtype.cpp \
    ../Source/Tome/Features/Types/View/enumerationwindow.cpp \
    ../Source/Tome/Features/Types/View/enumerationmemberwindow.cpp \
    ../Source/Tome/Features/Types/View/listwindow.cpp \
    ../Source/Tome/Features/Fields/View/listwidget.cpp \
    ../Source/Tome/Features/Fields/View/listitemwindow.cpp \
    ../Source/Tome/Features/Components/Controller/componentscontroller.cpp \
    ../Source/Tome/Core/controller.cpp \
    ../Source/Tome/Features/Export/Controller/exportcontroller.cpp \
    ../Source/Tome/Features/Records/Controller/recordscontroller.cpp \
    ../Source/Tome/Features/Fields/Controller/fielddefinitionscontroller.cpp \
    ../Source/Tome/Features/Types/Controller/typescontroller.cpp \
    ../Source/Tome/Features/Settings/Controller/settingscontroller.cpp \
    ../Source/Tome/Features/Records/View/recordtreewidgetitem.cpp \
    ../Source/Tome/Features/Records/View/recordtreewidget.cpp \
    ../Source/Tome/Features/Tasks/Controller/task.cpp \
    ../Source/Tome/Features/Tasks/Controller/taskscontroller.cpp \
    ../Source/Tome/Features/Tasks/Model/message.cpp \
    ../Source/Tome/Features/Tasks/Model/taskcontext.cpp \
    ../Source/Tome/Features/Integrity/Controller/fieldtypedoesnotexisttask.cpp \
    ../Source/Tome/Features/Integrity/Controller/listitemtypedoesnotexisttask.cpp \
    ../Source/Tome/Features/Tasks/View/errorlistdockwidget.cpp \
    ../Source/Tome/Features/Records/View/recordfieldstablewidget.cpp \
    ../Source/Tome/Features/Fields/View/vector2iwidget.cpp \
    ../Source/Tome/Features/Fields/View/vector3iwidget.cpp \
    ../Source/Tome/Features/Fields/View/vector2rwidget.cpp \
    ../Source/Tome/Features/Fields/View/vector3rwidget.cpp \
    ../Source/Tome/Features/Integrity/Controller/listitemtypenotsupportedtask.cpp \
    ../Source/Tome/Features/Search/Controller/findusagescontroller.cpp \
    ../Source/Tome/Features/Search/View/searchresultsdockwidget.cpp \
    ../Source/Tome/Features/Search/Model/searchresult.cpp \
    ../Source/Tome/Features/Records/View/duplicaterecordwindow.cpp \
    ../Source/Tome/Core/commandlineoptions.cpp \
    ../Source/Tome/Features/Types/View/mapwindow.cpp \
    ../Source/Tome/Features/Fields/View/mapitemwindow.cpp \
    ../Source/Tome/Features/Fields/View/mapwidget.cpp \
    ../Source/Tome/Features/Integrity/Controller/mapkeytypedoesnotexisttask.cpp \
    ../Source/Tome/Features/Integrity/Controller/mapkeytypenotsupportedtask.cpp \
    ../Source/Tome/Features/Integrity/Controller/mapvaluetypedoesnotexisttask.cpp \
    ../Source/Tome/Features/Integrity/Controller/mapvaluetypenotsupportedtask.cpp

HEADERS  += ../Source/Tome/Core/mainwindow.h \
    ../Source/Tome/Features/Types/Model/builtintype.h \
    ../Source/Tome/Features/Fields/Model/fielddefinition.h \
    ../Source/Tome/Features/Fields/Model/fielddefinitionset.h \
    ../Source/Tome/Features/Fields/Controller/fielddefinitionsetserializer.h \
    ../Source/Tome/Features/Help/View/aboutwindow.h \
    ../Source/Tome/Features/Projects/View/newprojectwindow.h \
    ../Source/Tome/Features/Projects/Controller/projectserializer.h \
    ../Source/Tome/Features/Projects/Model/project.h \
    ../Source/Tome/Features/Records/Model/record.h \
    ../Source/Tome/Features/Records/Model/recordset.h \
    ../Source/Tome/Features/Records/Controller/recordsetserializer.h \
    ../Source/Tome/Util/pathutils.h \
    ../Source/Tome/IO/xmlreader.h \
    ../Source/Tome/Features/Fields/View/fielddefinitionwindow.h \
    ../Source/Tome/Features/Fields/View/fielddefinitionswindow.h \
    ../Source/Tome/Features/Records/View/recordwindow.h \
    ../Source/Tome/Features/Fields/View/fieldvaluewindow.h \
    ../Source/Tome/Features/Fields/View/fieldvaluewidget.h \
    ../Source/Tome/Features/Export/Model/recordexporttemplate.h \
    ../Source/Tome/Features/Components/View/componentswindow.h \
    ../Source/Tome/Features/Components/View/componentwindow.h \
    ../Source/Tome/Features/Types/View/customtypeswindow.h \
    ../Source/Tome/Features/Types/Model/customtype.h \
    ../Source/Tome/Features/Types/View/enumerationmemberwindow.h \
    ../Source/Tome/Features/Types/View/enumerationwindow.h \
    ../Source/Tome/Features/Types/View/listwindow.h \
    ../Source/Tome/Features/Fields/View/listwidget.h \
    ../Source/Tome/Features/Fields/View/listitemwindow.h \
    ../Source/Tome/Features/Components/Controller/componentscontroller.h \
    ../Source/Tome/Features/Components/Model/component.h \
    ../Source/Tome/Core/controller.h \
    ../Source/Tome/Features/Components/Model/componentlist.h \
    ../Source/Tome/Features/Export/Controller/exportcontroller.h \
    ../Source/Tome/Features/Records/Controller/recordscontroller.h \
    ../Source/Tome/Features/Records/Model/recordlist.h \
    ../Source/Tome/Features/Records/Model/recordsetlist.h \
    ../Source/Tome/Features/Fields/Controller/fielddefinitionscontroller.h \
    ../Source/Tome/Features/Fields/Model/fielddefinitionsetlist.h \
    ../Source/Tome/Features/Fields/Model/fielddefinitionlist.h \
    ../Source/Tome/Features/Types/Controller/typescontroller.h \
    ../Source/Tome/Features/Types/Model/customtypelist.h \
    ../Source/Tome/Features/Settings/Controller/settingscontroller.h \
    ../Source/Tome/Util/listutils.h \
    ../Source/Tome/Features/Export/Model/recordexporttemplatemap.h \
    ../Source/Tome/Util/memoryutils.h \
    ../Source/Tome/Util/stringutils.h \
    ../Source/Tome/Features/Records/View/recordtreewidgetitem.h \
    ../Source/Tome/Features/Records/View/recordtreewidget.h \
    ../Source/Tome/Features/Records/Model/recordfieldvaluemap.h \
    ../Source/Tome/Features/Records/Model/recordfieldstate.h \
    ../Source/Tome/Features/Tasks/Controller/task.h \
    ../Source/Tome/Features/Tasks/Controller/taskscontroller.h \
    ../Source/Tome/Features/Tasks/Model/message.h \
    ../Source/Tome/Features/Tasks/Model/messagelist.h \
    ../Source/Tome/Features/Tasks/Model/severity.h \
    ../Source/Tome/Features/Tasks/Model/targetsitetype.h \
    ../Source/Tome/Features/Tasks/Model/taskcontext.h \
    ../Source/Tome/Features/Integrity/Controller/fieldtypedoesnotexisttask.h \
    ../Source/Tome/Features/Integrity/Controller/listitemtypedoesnotexisttask.h \
    ../Source/Tome/Features/Tasks/View/errorlistdockwidget.h \
    ../Source/Tome/Features/Records/View/recordfieldstablewidget.h \
    ../Source/Tome/Features/Types/Model/vector.h \
    ../Source/Tome/Features/Fields/View/vector2iwidget.h \
    ../Source/Tome/Features/Fields/View/vector3iwidget.h \
    ../Source/Tome/Features/Fields/View/vector2rwidget.h \
    ../Source/Tome/Features/Fields/View/vector3rwidget.h \
    ../Source/Tome/Features/Integrity/Controller/listitemtypenotsupportedtask.h \
    ../Source/Tome/Features/Search/Controller/findusagescontroller.h \
    ../Source/Tome/Features/Search/Model/searchresult.h \
    ../Source/Tome/Features/Search/Model/searchresultlist.h \
    ../Source/Tome/Features/Search/View/searchresultsdockwidget.h \
    ../Source/Tome/Features/Records/View/duplicaterecordwindow.h \
    ../Source/Tome/Core/commandlineoptions.h \
    ../Source/Tome/Features/Types/View/mapwindow.h \
    ../Source/Tome/Features/Fields/View/mapitemwindow.h \
    ../Source/Tome/Features/Fields/View/mapwidget.h \
    ../Source/Tome/Features/Integrity/Controller/mapkeytypedoesnotexisttask.h \
    ../Source/Tome/Features/Integrity/Controller/mapkeytypenotsupportedtask.h \
    ../Source/Tome/Features/Integrity/Controller/mapvaluetypedoesnotexisttask.h \
    ../Source/Tome/Features/Integrity/Controller/mapvaluetypenotsupportedtask.h

FORMS    += ../Source/Tome/Core/mainwindow.ui \
    ../Source/Tome/Features/Help/View/aboutwindow.ui \
    ../Source/Tome/Features/Projects/View/newprojectwindow.ui \
    ../Source/Tome/Features/Fields/View/fielddefinitionwindow.ui \
    ../Source/Tome/Features/Fields/View/fielddefinitionswindow.ui \
    ../Source/Tome/Features/Records/View/recordwindow.ui \
    ../Source/Tome/Features/Fields/View/fieldvaluewindow.ui \
    ../Source/Tome/Features/Components/View/componentswindow.ui \
    ../Source/Tome/Features/Components/View/componentwindow.ui \
    ../Source/Tome/Features/Types/View/customtypeswindow.ui \
    ../Source/Tome/Features/Types/View/enumerationwindow.ui \
    ../Source/Tome/Features/Types/View/enumerationmemberwindow.ui \
    ../Source/Tome/Features/Types/View/listwindow.ui \
    ../Source/Tome/Features/Fields/View/listitemwindow.ui \
    ../Source/Tome/Features/Records/View/duplicaterecordwindow.ui \
    ../Source/Tome/Features/Types/View/mapwindow.ui \
    ../Source/Tome/Features/Fields/View/mapitemwindow.ui

RESOURCES += \
    tome.qrc
