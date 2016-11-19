#ifndef SEARCHRESULTSDOCKWIDGET_H
#define SEARCHRESULTSDOCKWIDGET_H

#include <QDockWidget>
#include <QTableWidget>
#include <QVBoxLayout>

#include "../Model/searchresultlist.h"

namespace Tome
{
    class SearchResultsDockWidget : public QDockWidget
    {
            Q_OBJECT

        public:
            SearchResultsDockWidget(QWidget* parent);
            ~SearchResultsDockWidget();

            void showResults(const QString& title, const SearchResultList& results);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);
            void recordLinkActivated(const QString& recordId);

        private:
            QWidget* widget;
            QVBoxLayout* verticalLayout;
            QTableWidget* tableWidget;

            SearchResultList results;

        private slots:
            void onRecordLinkActivated(const QString& recordId);
    };
}

#endif // SEARCHRESULTSDOCKWIDGET_H
