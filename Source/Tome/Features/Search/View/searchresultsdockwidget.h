#ifndef SEARCHRESULTSDOCKWIDGET_H
#define SEARCHRESULTSDOCKWIDGET_H

#include <QDockWidget>
#include <QTableWidget>
#include <QVBoxLayout>

#include "../Model/searchresultlist.h"

namespace Tome
{
    /**
     * @brief Shows the results of a search operation.
     */
    class SearchResultsDockWidget : public QDockWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for showing the results of a search operation.
             * @param parent Optional owner widget.
             */
            SearchResultsDockWidget(QWidget* parent);
            ~SearchResultsDockWidget();

            /**
             * @brief Shows the passed search results.
             * @param title Title of the search operation (e.g. what we've been looking for).
             * @param results Search results to show.
             */
            void showResults(const QString& title, const SearchResultList& results);

        signals:
            /**
             * @brief Progress of the current search operation has changed.
             * @param title Title of the current search operation step.
             * @param text Details of the current search operation step.
             * @param currentValue Current progress value of the current search operation step.
             * @param maximumValue Maximum progress value of the current search operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);

            /**
             * @brief A link to another Tome record has been activated.
             * @param recordId Id of the selected record.
             */
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
