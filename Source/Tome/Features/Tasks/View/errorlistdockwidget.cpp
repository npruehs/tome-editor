#include "errorlistdockwidget.h"

#include <QHeaderView>
#include <QLabel>
#include <QString>

using namespace Tome;

ErrorListDockWidget::ErrorListDockWidget(QWidget* parent) :
    QDockWidget(tr("Error List"), parent)
{
    this->widget = new QWidget(this);
    this->verticalLayout = new QVBoxLayout();

    // Add tool buttons.
    this->toolButtonsLayout = new QHBoxLayout();

    this->toolButtonErrors = new QToolButton();
    this->toolButtonErrors->setText(tr("Errors"));
    this->toolButtonErrors->setIcon(QIcon(":/Error"));
    this->toolButtonErrors->setCheckable(true);
    this->toolButtonErrors->setChecked(true);
    this->toolButtonErrors->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->toolButtonMessages = new QToolButton();
    this->toolButtonMessages->setText(tr("Messages"));
    this->toolButtonMessages->setIcon(QIcon(":/Information"));
    this->toolButtonMessages->setCheckable(true);
    this->toolButtonMessages->setChecked(true);
    this->toolButtonMessages->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->toolButtonWarnings = new QToolButton();
    this->toolButtonWarnings->setText(tr("Warnings"));
    this->toolButtonWarnings->setIcon(QIcon(":/Warning"));
    this->toolButtonWarnings->setCheckable(true);
    this->toolButtonWarnings->setChecked(true);
    this->toolButtonWarnings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->toolButtonsLayout->addWidget(this->toolButtonErrors);
    this->toolButtonsLayout->addWidget(this->toolButtonWarnings);
    this->toolButtonsLayout->addWidget(this->toolButtonMessages);

    this->toolButtonsLayout->addStretch(1);

    this->verticalLayout->addLayout(this->toolButtonsLayout);

    // Add error list.
    this->tableWidgetErrorList = new QTableWidget();
    this->tableWidgetErrorList->setEditTriggers(QTableWidget::NoEditTriggers);
    this->tableWidgetErrorList->setSelectionMode(QTableWidget::SingleSelection);
    this->tableWidgetErrorList->setSelectionBehavior(QTableWidget::SelectRows);
    this->tableWidgetErrorList->horizontalHeader()->setStretchLastSection(true);
    this->tableWidgetErrorList->verticalHeader()->setVisible(false);
    this->tableWidgetErrorList->setSortingEnabled(true);

    this->verticalLayout->addWidget(this->tableWidgetErrorList);

    // Finish layout.
    this->widget->setLayout(this->verticalLayout);
    this->setWidget(this->widget);

    // Connect signals.
    connect(
                this->toolButtonErrors,
                SIGNAL(toggled(bool)),
                SLOT(onToolButtonErrorsToggled(bool))
                );

    connect(
                this->toolButtonMessages,
                SIGNAL(toggled(bool)),
                SLOT(onToolButtonMessagesToggled(bool))
                );

    connect(
                this->toolButtonWarnings,
                SIGNAL(toggled(bool)),
                SLOT(onToolButtonWarningsToggled(bool))
                );
}

ErrorListDockWidget::~ErrorListDockWidget()
{
    delete this->tableWidgetErrorList;

    delete this->toolButtonWarnings;
    delete this->toolButtonMessages;
    delete this->toolButtonErrors;

    delete this->toolButtonsLayout;

    delete this->verticalLayout;

    delete this->widget;
}

void ErrorListDockWidget::showMessages(const MessageList& messages)
{
    this->messages = messages;
    this->refreshMessages();
}

void ErrorListDockWidget::refreshMessages()
{
    // Reset output window.
    this->tableWidgetErrorList->setRowCount(this->messages.count());
    this->tableWidgetErrorList->setColumnCount(4);

    // Show results.
    int messagesShown = 0;

    for (int i = 0; i < this->messages.count(); ++i)
    {
        const Message message = this->messages.at(i);

        // Update progress bar.
        if (i % 10 == 0)
        {
            emit this->progressChanged(tr("Refreshing Error List"), QString(), i, this->messages.count());
        }

        // Check filter.
        switch (message.severity)
        {
            case Severity::Error:
                if (!this->toolButtonErrors->isChecked())
                {
                    continue;
                }
                break;

            case Severity::Warning:
                if (!this->toolButtonWarnings->isChecked())
                {
                    continue;
                }
                break;

            case Severity::Information:
                if (!this->toolButtonMessages->isChecked())
                {
                    continue;
                }
                break;

            default:
                break;
        }

        // Show severity.
        this->tableWidgetErrorList->setItem(messagesShown, 0, new QTableWidgetItem(Severity::toString(message.severity)));

        switch (message.severity)
        {
            case Severity::Error:
                this->tableWidgetErrorList->item(messagesShown, 0)->setData(Qt::DecorationRole, QIcon(":/Error"));
                break;

            case Severity::Warning:
                this->tableWidgetErrorList->item(messagesShown, 0)->setData(Qt::DecorationRole, QIcon(":/Warning"));
                break;

            case Severity::Information:
                this->tableWidgetErrorList->item(messagesShown, 0)->setData(Qt::DecorationRole, QIcon(":/Information"));
                break;

            default:
                break;
        }

        // Show help link.
        QString helpLink = message.helpLink.isEmpty() ? "https://github.com/npruehs/tome-editor/wiki/" + message.messageCode : message.helpLink;
        QLabel* helpLinkLabel = new QLabel("<a href=\"" + helpLink + "\">" + message.messageCode + "</a>");
        helpLinkLabel->setToolTip(helpLink);
        helpLinkLabel->setOpenExternalLinks(true);

        QModelIndex index = this->tableWidgetErrorList->model()->index(messagesShown, 1);
        this->tableWidgetErrorList->setIndexWidget(index, helpLinkLabel);

        // Show message.
        this->tableWidgetErrorList->setItem(messagesShown, 2, new QTableWidgetItem(message.content));

        // Show location.
        QLabel* locationLabel;

        if (message.targetSiteType == TargetSiteType::Record)
        {
            QString locationLink = QString("Record - <a href='%1'>%1</a>").arg(message.targetSiteId);
            locationLabel = new QLabel(locationLink);

            connect(
                        locationLabel,
                        SIGNAL(linkActivated(const QString&)),
                        SLOT(onRecordLinkActivated(const QString&))
                        );
        }
        else
        {
            QString locationString = TargetSiteType::toString(message.targetSiteType) + " - " + message.targetSiteId;
            locationLabel = new QLabel(locationString);
        }

        index = this->tableWidgetErrorList->model()->index(messagesShown, 3);
        this->tableWidgetErrorList->setIndexWidget(index, locationLabel);

        // Increase row counter.
        ++messagesShown;
    }

    this->tableWidgetErrorList->setRowCount(messagesShown);

    // Add headers.
    QStringList headers;
    headers << tr("Severity");
    headers << tr("Code");
    headers << tr("Message");
    headers << tr("Location");

    this->tableWidgetErrorList->setHorizontalHeaderLabels(headers);
    this->tableWidgetErrorList->resizeColumnsToContents();

    emit this->progressChanged(tr("Refreshing Error List"), QString(), 1, 1);
}

void ErrorListDockWidget::onRecordLinkActivated(const QString& recordId)
{
    emit this->recordLinkActivated(recordId);
}

void ErrorListDockWidget::onToolButtonErrorsToggled(bool checked)
{
    Q_UNUSED(checked)
    this->refreshMessages();
}

void ErrorListDockWidget::onToolButtonWarningsToggled(bool checked)
{
    Q_UNUSED(checked)
    this->refreshMessages();
}

void ErrorListDockWidget::onToolButtonMessagesToggled(bool checked)
{
    Q_UNUSED(checked)
    this->refreshMessages();
}
