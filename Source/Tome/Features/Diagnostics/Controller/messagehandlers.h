#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include <QList>
#include <QtMessageHandler>

namespace Tome
{
    class MessageHandlers
    {
        public:
            static void addMessageHandler(QtMessageHandler handler);

        private:
            static QList<QtMessageHandler> handlers;

            static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    };
}

#endif // MESSAGEHANDLERS_H
