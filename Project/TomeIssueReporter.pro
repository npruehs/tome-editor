#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T01:17:57
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TomeIssueReporter
TEMPLATE = app

RC_ICONS = ../Media/Icons/Tome.ico
ICON = ../Media/Icons/Tome.icns

Debug:DESTDIR = ../../Bin/debug/TomeIssueReporter
Debug:OBJECTS_DIR = ../../Obj/debug/TomeIssueReporter
Debug:MOC_DIR = ../../Obj/debug/TomeIssueReporter/.moc
Debug:RCC_DIR = ../../Obj/debug/TomeIssueReporter/.rcc
Debug:UI_DIR = ../../Obj/debug/TomeIssueReporter/.ui

Release:DESTDIR = ../../Bin/release/TomeIssueReporter
Release:OBJECTS_DIR = ../../Obj/release/TomeIssueReporter
Release:MOC_DIR = ../../Obj/release/TomeIssueReporter/.moc
Release:RCC_DIR = ../../Obj/release/TomeIssueReporter/.rcc
Release:UI_DIR = ../../Obj/release/TomeIssueReporter/.ui

SOURCES += ../Source/TomeIssueReporter/main.cpp\
        ../Source/TomeIssueReporter/mainwindow.cpp

HEADERS  += ../Source/TomeIssueReporter/mainwindow.h \
    ../Source/TomeIssueReporter/config.h

FORMS    += ../Source/TomeIssueReporter/mainwindow.ui
