#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDockWidget>
#include <QVBoxLayout>
#include <QTextBrowser>

namespace Tome
{
    /**
     * @brief Dock widget for showing Qt messages (e.g. qInfo or qCritical).
     */
    class OutputDockWidget : public QDockWidget
    {
        public:
            /**
             * @brief Constructs a new dock widget for showing Qt messages.
             * @param parent Optional owner widget.
             */
            OutputDockWidget(QWidget* parent);
            ~OutputDockWidget();

            /**
             * @brief Sets up this dock widget for showing Qt messages.
             */
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
