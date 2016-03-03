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
        public:
            ErrorListDockWidget(QWidget* parent);
            ~ErrorListDockWidget();

            void showMessages(const MessageList& messages);

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
    };
}


#endif // ERRORLISTDOCKWIDGET_H
