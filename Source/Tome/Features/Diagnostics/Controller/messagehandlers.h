#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include <QList>
#include <QtMessageHandler>

namespace Tome
{
    /**
     * @brief Manages all registered handlers for Qt messages (e.g. qInfo or qCritical).
     */
    class MessageHandlers
    {
        public:
            /**
             * @brief Appends a new handler for Qt messages, in addition to the default handler.
             * @param handler Handler to append.
             */
            static void addMessageHandler(QtMessageHandler handler);

        private:
            static QList<QtMessageHandler> handlers;

            static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    };
}

#endif // MESSAGEHANDLERS_H
