#include <QtTest/QtTest>

#include "Tests/testlistutils.h"
#include "Tests/teststringutils.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    TestListUtils testListUtils;
    TestStringUtils testStringUtils;

    return QTest::qExec(&testListUtils, argc, argv) &
           QTest::qExec(&testStringUtils, argc, argv);
}
