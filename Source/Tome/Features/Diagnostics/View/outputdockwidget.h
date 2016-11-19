#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDockWidget>
#include <QVBoxLayout>
#include <QTextBrowser>

namespace Tome
{
    class OutputDockWidget : public QDockWidget
    {
        public:
            OutputDockWidget(QWidget* parent);
            ~OutputDockWidget();

            void init();

        private:
            static OutputDockWidget* logDockWidget;

            QWidget* widget;
            QVBoxLayout* verticalLayout;
            QTextBrowser* textBrowser;

            static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    };
}

#endif // LOGWINDOW_H
