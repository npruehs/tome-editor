#include "testlistutils.h"

#include "../Util/listutils.h"

using namespace Tome;


inline bool qStringLessThanLowerCase(const QString& e1, const QString& e2)
{
    return e1.toLower() < e2.toLower();
}


void TestListUtils::findInsertionIndexFirst()
{
    // ARRANGE.
    QStringList strings;
    strings << "b";
    strings << "c";
    strings << "d";

    QString string = "a";

    // ACT.
    int insertionIndex = findInsertionIndex(strings, string, qStringLessThanLowerCase);

    // ASSERT.
    QCOMPARE(insertionIndex, 0);
}
