#ifndef FILEMESSAGEHANDLER_H
#define FILEMESSAGEHANDLER_H

#include <QFile>
#include <QtMessageHandler>

namespace Tome
{
    /**
     * @brief Writes Qt messages (e.g. qInfo or qCritical) to a log file.
     */
    class FileMessageHandler
    {
        public:
            /**
             * @brief Opens and clears the log file for writing.
             * @return true, if opening the log file was successful, and false otherwise.
             */
            static bool init();

            /**
             * @brief Writes the passed message to the log file.
             * @param type Severity of the message to write.
             * @param context Source code file and line number of the message origin.
             * @param msg Message to write.
             */
            static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

            /**
             * @brief Path to the log file.
             */
            static const QString logfileName;

        private:
            static bool initialized;
            static QFile logFile;
    };
}

#endif // FILEMESSAGEHANDLER_H
