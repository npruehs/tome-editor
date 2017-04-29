#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QAbstractMessageHandler>
#include <QTextDocument>

class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler() : QAbstractMessageHandler(nullptr)
    {
    }

    QtMsgType getMessageType() const
    {
        return this->messageType;
    }

    QString getDescription() const
    {
        return this->messageDescription;
    }

    QUrl getIdentifier() const
    {
        return this->messageIdentifier;
    }

    QSourceLocation getSourceLocation() const
    {
        return this->messageSourceLocation;
    }

protected:
    virtual void handleMessage(QtMsgType type,
                               const QString &description,
                               const QUrl &identifier,
                               const QSourceLocation &sourceLocation)
    {
        messageType = type;
        messageIdentifier = identifier;
        messageSourceLocation = sourceLocation;

        QTextDocument textDocument;
        textDocument.setHtml(description);
        messageDescription = textDocument.toPlainText();
    }

private:
    QtMsgType messageType;
    QString messageDescription;
    QUrl messageIdentifier;
    QSourceLocation messageSourceLocation;
};

#endif // MESSAGEHANDLER_H
