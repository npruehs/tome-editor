#ifndef ERRORLISTDOCKWIDGET_H
#define ERRORLISTDOCKWIDGET_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "../Model/messagelist.h"

namespace Tome
{
    class ErrorListDockWidget : public QDockWidget
    {
        Q_OBJECT

        public:
            ErrorListDockWidget(QWidget* parent);
            ~ErrorListDockWidget();

            void showMessages(const MessageList& messages);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void recordLinkActivated(const QString& recordId);

        private:
            QWidget* widget;

            QVBoxLayout* verticalLayout;

            QTableWidget* tableWidgetErrorList;

            QHBoxLayout* toolButtonsLayout;

            QToolButton* toolButtonErrors;
            QToolButton* toolButtonMessages;
            QToolButton* toolButtonWarnings;

            MessageList messages;

            void refreshMessages();

        private slots:
            void onRecordLinkActivated(const QString& recordId);
            void onToolButtonErrorsToggled(bool checked);
            void onToolButtonWarningsToggled(bool checked);
            void onToolButtonMessagesToggled(bool checked);
    };
}


#endif // ERRORLISTDOCKWIDGET_H
