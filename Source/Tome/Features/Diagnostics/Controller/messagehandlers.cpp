#include "messagehandlers.h"

using namespace Tome;


QList<QtMessageHandler> MessageHandlers::handlers = QList<QtMessageHandler>();


void MessageHandlers::addMessageHandler(QtMessageHandler handler)
{
    if (handlers.empty())
    {
        // Replace default Qt message handler with this one.
        QtMessageHandler defaultHandler = qInstallMessageHandler(handleMessage);
        handlers.append(defaultHandler);
    }

    handlers.append(handler);
}

void MessageHandlers::handleMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Relay message to all handlers.
    for (int i = 0; i < handlers.count(); ++i)
    {
        handlers[i](type, context, msg);
    }
}
