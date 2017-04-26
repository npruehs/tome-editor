#ifndef TESTSTRINGUTILS_H
#define TESTSTRINGUTILS_H

#include <QtTest/QtTest>


class TestStringUtils : public QObject
{
    Q_OBJECT

    private slots:
        void containsWhitespacesNone();
        void containsWhitespacesOneSpace();
        void containsWhitespacesOneLeadingSpace();
        void containsWhitespacesOneTrailingSpace();
        void containsWhitespacesTwoSpaces();
        void containsWhitespacesTwoAdjacentSpaces();
        void containsWhitespacesOneTab();
        void containsWhitespacesTwoTabs();
        void containsWhitespacesTwoAdjacentTabs();

        void splitAtCapitalLettersNone();
        void splitAtCapitalLettersOne();
        void splitAtCapitalLettersTwo();

        void stripWhitespacesNone();
        void stripWhitespacesOneSpace();
        void stripWhitespacesOneLeadingSpace();
        void stripWhitespacesOneTrailingSpace();
        void stripWhitespacesTwoSpaces();
        void stripWhitespacesTwoAdjacentSpaces();
        void stripWhitespacesOneTab();
        void stripWhitespacesTwoTabs();
        void stripWhitespacesTwoAdjacentTabs();
};

#endif // TESTSTRINGUTILS_H
