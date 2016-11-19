#ifndef FILEMESSAGEHANDLER_H
#define FILEMESSAGEHANDLER_H

#include <QFile>
#include <QtMessageHandler>

namespace Tome
{
    class FileMessageHandler
    {
        public:
            static bool init();
            static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

        private:
            static bool initialized;
            static QFile logFile;
    };
}

#endif // FILEMESSAGEHANDLER_H
