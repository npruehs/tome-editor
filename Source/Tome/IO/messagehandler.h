#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QAbstractMessageHandler>
#include <QTextDocument>

/**
 * @brief Message handler that stores the most recent message.
 */
class MessageHandler : public QAbstractMessageHandler
{
public:
    /**
     * @brief Constructs a new message handler that stores the most recent message.
     */
    MessageHandler() : QAbstractMessageHandler(nullptr)
    {
    }

    /**
     * @brief Gets the type of the most recent message.
     * @return Type of the most recent message.
     */
    QtMsgType getMessageType() const
    {
        return this->messageType;
    }

    /**
     * @brief Gets the text of the most recent message.
     * @return Text of the most recent message.
     */
    QString getDescription() const
    {
        return this->messageDescription;
    }

    /**
     * @brief Gets the URL of the most recent message.
     * @return URL of the most recent message.
     */
    QUrl getIdentifier() const
    {
        return this->messageIdentifier;
    }

    /**
     * @brief Gets the source location of the most recent message.
     * @return Source location of the most recent message.
     */
    QSourceLocation getSourceLocation() const
    {
        return this->messageSourceLocation;
    }

protected:
    /**
     * @brief Stores the specified message.
     * @param type Type of the message to store.
     * @param description Text of the message to store.
     * @param identifier URL of the message to store.
     * @param sourceLocation Source location of the message to store.
     */
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
