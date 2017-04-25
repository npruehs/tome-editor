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

void TestListUtils::findInsertionIndexMid()
{
    // ARRANGE.
    QStringList strings;
    strings << "a";
    strings << "c";
    strings << "d";

    QString string = "b";

    // ACT.
    int insertionIndex = findInsertionIndex(strings, string, qStringLessThanLowerCase);

    // ASSERT.
    QCOMPARE(insertionIndex, 1);
}

void TestListUtils::findInsertionIndexLast()
{
    // ARRANGE.
    QStringList strings;
    strings << "a";
    strings << "b";
    strings << "c";

    QString string = "d";

    // ACT.
    int insertionIndex = findInsertionIndex(strings, string, qStringLessThanLowerCase);

    // ASSERT.
    QCOMPARE(insertionIndex, 3);
}

void TestListUtils::toStringEmptyList()
{
    // ARRANGE.
    QVariantList numbers;

    // ACT.
    QString listString = toString(numbers);

    // ASSERT.
    QCOMPARE(listString, QString("[]"));
}

void TestListUtils::toStringOneElementList()
{
    // ARRANGE.
    QVariantList numbers;
    numbers << 1;

    // ACT.
    QString listString = toString(numbers);

    // ASSERT.
    QCOMPARE(listString, QString("[1]"));
}

void TestListUtils::toStringThreeElementsList()
{
    // ARRANGE.
    QVariantList numbers;
    numbers << 1;
    numbers << 2;
    numbers << 3;

    // ACT.
    QString listString = toString(numbers);

    // ASSERT.
    QCOMPARE(listString, QString("[1, 2, 3]"));
}

void TestListUtils::toStringEmptyMap()
{
    // ARRANGE.
    QVariantMap numbers;

    // ACT.
    QString mapString = toString(numbers);

    // ASSERT.
    QCOMPARE(mapString, QString("[]"));
}

void TestListUtils::toStringOneElementMap()
{
    // ARRANGE.
    QVariantMap numbers;
    numbers.insert("one", 1);

    // ACT.
    QString mapString = toString(numbers);

    // ASSERT.
    QCOMPARE(mapString, QString("[{one:1}]"));
}
