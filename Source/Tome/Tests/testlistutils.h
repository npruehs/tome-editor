#ifndef TESTLISTUTILS_H
#define TESTLISTUTILS_H

#include <QtTest/QtTest>


class TestListUtils : public QObject
{
    Q_OBJECT

    private slots:
        void findInsertionIndexFirst();
        void findInsertionIndexMid();
        void findInsertionIndexLast();

        void toStringEmptyList();
        void toStringOneElementList();
        void toStringThreeElementsList();

        void toStringEmptyMap();
        void toStringOneElementMap();
};

#endif // TESTLISTUTILS_H
