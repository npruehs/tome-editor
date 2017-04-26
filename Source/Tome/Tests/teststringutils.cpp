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

void TestStringUtils::splitAtCapitalLettersNone()
{
    // ARRANGE.
    QString string = "Onetwothree";

    // ACT.
    QString split = splitAtCapitalLetters(string);

    // ASSERT.
    QCOMPARE(split, QString("Onetwothree"));
}

void TestStringUtils::splitAtCapitalLettersOne()
{
    // ARRANGE.
    QString string = "OneTwothree";

    // ACT.
    QString split = splitAtCapitalLetters(string);

    // ASSERT.
    QCOMPARE(split, QString("One Twothree"));
}

void TestStringUtils::splitAtCapitalLettersTwo()
{
    // ARRANGE.
    QString string = "OneTwoThree";

    // ACT.
    QString split = splitAtCapitalLetters(string);

    // ASSERT.
    QCOMPARE(split, QString("One Two Three"));
}

void TestStringUtils::stripWhitespacesNone()
{
    // ARRANGE.
    QString string = "ab";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesOneSpace()
{
    // ARRANGE.
    QString string = "a b";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesOneLeadingSpace()
{
    // ARRANGE.
    QString string = " ab";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesOneTrailingSpace()
{
    // ARRANGE.
    QString string = "ab ";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesTwoSpaces()
{
    // ARRANGE.
    QString string = "a b c";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("abc"));
}

void TestStringUtils::stripWhitespacesTwoAdjacentSpaces()
{
    // ARRANGE.
    QString string = "a  b";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesOneTab()
{
    // ARRANGE.
    QString string = "a\tb";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}

void TestStringUtils::stripWhitespacesTwoTabs()
{
    // ARRANGE.
    QString string = "a\tb\tc";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("abc"));
}

void TestStringUtils::stripWhitespacesTwoAdjacentTabs()
{
    // ARRANGE.
    QString string = "a\t\tb";

    // ACT.
    QString stripped = stripWhitespaces(string);

    // ASSERT.
    QCOMPARE(stripped, QString("ab"));
}
