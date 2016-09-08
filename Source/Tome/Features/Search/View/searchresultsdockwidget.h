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
        public:
            SearchResultsDockWidget(QWidget* parent);
            ~SearchResultsDockWidget();

            void showResults(const QString& title, const SearchResultList& results);

        private:
            QWidget* widget;
            QVBoxLayout* verticalLayout;
            QTableWidget* tableWidget;

            SearchResultList results;
    };
}

#endif // SEARCHRESULTSDOCKWIDGET_H
