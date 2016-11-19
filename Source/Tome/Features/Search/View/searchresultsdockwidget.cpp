#include "searchresultsdockwidget.h"

#include <QHeaderView>
#include <QLabel>

#include "../Model/searchresult.h"


using namespace Tome;


SearchResultsDockWidget::SearchResultsDockWidget(QWidget* parent) :
    QDockWidget(tr("Search Results"), parent)
{
    this->widget = new QWidget(this);
    this->verticalLayout = new QVBoxLayout();

    this->tableWidget = new QTableWidget();
    this->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    this->tableWidget->setSelectionMode(QTableWidget::SingleSelection);
    this->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    this->tableWidget->horizontalHeader()->setStretchLastSection(true);
    this->tableWidget->horizontalHeader()->setVisible(false);
    this->tableWidget->verticalHeader()->setVisible(false);
    this->tableWidget->setSortingEnabled(true);

    this->verticalLayout->addWidget(this->tableWidget);

    // Finish layout.
    this->widget->setLayout(this->verticalLayout);
    this->setWidget(this->widget);
}

SearchResultsDockWidget::~SearchResultsDockWidget()
{
    delete this->tableWidget;
    delete this->verticalLayout;
    delete this->widget;
}

void SearchResultsDockWidget::showResults(const QString& title, const SearchResultList& results)
{
    this->setWindowTitle("Search Results - " + title + " - " + QString::number(results.size()) + " matches found");
    this->results = results;

    // Reset output window.
    this->tableWidget->setRowCount(this->results.count());
    this->tableWidget->setColumnCount(1);

    // Show results.
    for (int i = 0; i < this->results.count(); ++i)
    {
        const SearchResult& result = this->results.at(i);

        // Report progress.
        emit this->progressChanged(tr("Collecting results"), result.content, i, this->results.count());

        // Create table row.
        this->tableWidget->setItem(i, 0, new QTableWidgetItem());
        QLabel* resultLabel;

        // Show hyperlink for records, and normal text for other results.
        if (result.targetSiteType == TargetSiteType::Record)
        {
            QString resultString = QString("<a href='%1'>%1</a>: %2").arg(result.targetSiteId, result.content);
            resultLabel = new QLabel(resultString);

            connect(
                        resultLabel,
                        SIGNAL(linkActivated(const QString&)),
                        SLOT(onRecordLinkActivated(const QString&))
                        );
        }
        else
        {
            QString resultString = QString(result.targetSiteId + ": " + result.content);
            resultLabel = new QLabel(resultString);
        }

        QModelIndex index = this->tableWidget->model()->index(i, 0);
        this->tableWidget->setIndexWidget(index, resultLabel);
    }

    // Report finish.
    emit this->progressChanged(tr("Collecting results"), QString(), 1, 1);
}

void SearchResultsDockWidget::onRecordLinkActivated(const QString& recordId)
{
    emit this->recordLinkActivated(recordId);
}
