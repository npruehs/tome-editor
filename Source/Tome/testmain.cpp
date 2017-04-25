#include <QtTest/QtTest>

#include "Tests/testlistutils.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    TestListUtils testListUtils;

    return QTest::qExec(&testListUtils, argc, argv);
}
