#include "xmlreader.h"

#include <stdexcept>

#include <QFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include "messagehandler.h"


XmlReader::XmlReader(QIODevice* device)
    : device(device)
{
}

XmlReader::~XmlReader()
{
    delete this->reader;
}

QString XmlReader::getElementName() const
{
    return this->reader->name().toString();
}

bool XmlReader::isAtElement(const QString& elementName) const
{
    return this->getElementName() == elementName;
}

QString XmlReader::readAttribute(const QString& attributeName) const
{
    return this->reader->attributes().value(attributeName).toString();
}

void XmlReader::readEmptyElement(const QString& expectedElementName)
{
    this->readStartElement(expectedElementName);
    this->readEndElement();
}

void XmlReader::readEndDocument()
{
    this->readToken(QXmlStreamReader::EndDocument);
}

void XmlReader::readEndElement()
{
    this->readToken(QXmlStreamReader::EndElement);
}

void XmlReader::readStartDocument()
{
    this->reader = new QXmlStreamReader(device);
    this->reader->readNext();

    this->readToken(QXmlStreamReader::StartDocument);
}

void XmlReader::readStartElement(const QString& expectedElementName)
{
    const qint64 line = this->reader->lineNumber();
    const qint64 column = this->reader->columnNumber();

    // Verify token type.
    if (!this->reader->isStartElement())
    {
        throwTokenError(line, column);
    }

    // Verify element name.
    if (!this->isAtElement(expectedElementName))
    {
        QString errorMessage =
                "Expected " + expectedElementName + ", but was " + this->getElementName() + ".";
        throwTokenError(line, column, errorMessage);
    }

    // Advance to next token.
    this->moveToNextToken();
}

QString XmlReader::readTextElement(const QString& textElementName)
{
    // Read start element.
    this->readStartElement(textElementName);

    // Read text.
    QString text = this->reader->text().toString();

    // Read end element.
    this->reader->readNext();

    // Advance to next token.
    this->moveToNextToken();

    return text;
}

void XmlReader::validate(const QString& schemaFileName,
                         const QString& validationErrorMessage)
{
    // Load schema.
    QXmlSchema schema;

    MessageHandler messageHandler;
    schema.setMessageHandler(&messageHandler);

    QFile schemaFile(schemaFileName);

    if (!schemaFile.open(QFile::ReadOnly))
    {
        QString errorMessage = "Schema file could not be opened: " + schemaFileName;
        qCritical(qUtf8Printable(errorMessage));
        throw std::runtime_error(errorMessage.toStdString());
    }

    schema.load(&schemaFile);

    if (!schema.isValid())
    {
        QString errorMessage = "Schema file is invalid: " + schemaFileName;
        qCritical(qUtf8Printable(errorMessage));
        throw std::runtime_error(errorMessage.toStdString());
    }

    // Validate data.
    const QByteArray xmlData = this->device->readAll();

    QXmlSchemaValidator validator(schema);

    if (!validator.validate(xmlData))
    {
        QString errorMessage = validationErrorMessage.arg
                    (messageHandler.getDescription(),
                     QString::number(messageHandler.getSourceLocation().line()),
                     QString::number(messageHandler.getSourceLocation().column()));

        qCritical(qUtf8Printable(errorMessage));
        throw std::runtime_error(errorMessage.toStdString());
    }

    this->device->reset();
}

void XmlReader::moveToNextToken()
{
    this->reader->readNext();

    while (this->reader->isWhitespace())
    {
        this->reader->readNext();
    }
}

void XmlReader::readToken(const QXmlStreamReader::TokenType& expectedTokenType)
{
    const qint64 line = this->reader->lineNumber();
    const qint64 column = this->reader->columnNumber();

    // Verify token type.
    const QXmlStreamReader::TokenType tokenType = this->reader->tokenType();
    if (tokenType != expectedTokenType)
    {
        throwTokenError(line, column);
    }

    // Advance to next token.
    this->moveToNextToken();
}

void XmlReader::throwTokenError(const qint64& line, const qint64& column) const
{
    throwTokenError(line, column, "");
}

void XmlReader::throwTokenError(const qint64& line, const qint64& column, const QString& detailMessage) const
{
    QString errorMessage = "Invalid token at line " + QString::number(line) +
            ", column " + QString::number(column) + ".";

    if (detailMessage.count() > 0)
    {
        errorMessage.append("\r\n" + detailMessage);
    }

    throw std::runtime_error(errorMessage.toStdString());
}
