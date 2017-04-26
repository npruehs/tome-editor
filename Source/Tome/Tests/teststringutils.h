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
};

#endif // TESTSTRINGUTILS_H
