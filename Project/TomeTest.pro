#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T20:25:19
#
#-------------------------------------------------

include(Tome.pro)


QT += testlib

SOURCES -= ../Source/Tome/main.cpp

HEADERS += ../Source/Tome/Tests/testlistutils.h \
    ../Source/Tome/Tests/teststringutils.h

SOURCES += ../Source/Tome/testmain.cpp \
    ../Source/Tome/Tests/testlistutils.cpp \
    ../Source/Tome/Tests/teststringutils.cpp
