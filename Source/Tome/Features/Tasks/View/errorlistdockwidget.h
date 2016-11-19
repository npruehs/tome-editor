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

            void on_toolButtonErrors_toggled(bool checked);
            void on_toolButtonWarnings_toggled(bool checked);
            void on_toolButtonMessages_toggled(bool checked);

            void refreshMessages();

        private slots:
            void onRecordLinkActivated(const QString& recordId);
    };
}


#endif // ERRORLISTDOCKWIDGET_H
