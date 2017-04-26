#include "teststringutils.h"

#include "../Util/stringutils.h"

using namespace Tome;


void TestStringUtils::containsWhitespacesNone()
{
    // ARRANGE.
    QString string = "ab";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, false);
}

void TestStringUtils::containsWhitespacesOneSpace()
{
    // ARRANGE.
    QString string = "a b";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesOneLeadingSpace()
{
    // ARRANGE.
    QString string = " ab";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesOneTrailingSpace()
{
    // ARRANGE.
    QString string = "ab ";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesTwoSpaces()
{
    // ARRANGE.
    QString string = "a b c";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesTwoAdjacentSpaces()
{
    // ARRANGE.
    QString string = "a  b";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesOneTab()
{
    // ARRANGE.
    QString string = "a\tb";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesTwoTabs()
{
    // ARRANGE.
    QString string = "a\tb\tc";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}

void TestStringUtils::containsWhitespacesTwoAdjacentTabs()
{
    // ARRANGE.
    QString string = "a\t\tb";

    // ACT.
    bool contains = containsWhitespaces(string);

    // ASSERT.
    QCOMPARE(contains, true);
}
