#include "xmlreader.h"

#include <stdexcept>


XmlReader::XmlReader(QXmlStreamReader& reader)
    : reader(reader)
{
    this->reader.readNext();
}

QString XmlReader::getElementName() const
{
    return this->reader.name().toString();
}

bool XmlReader::isAtElement(const QString& elementName) const
{
    return this->getElementName() == elementName;
}

QString XmlReader::readAttribute(const QString& attributeName)
{
    return this->reader.attributes().value(attributeName).toString();
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
    this->readToken(QXmlStreamReader::StartDocument);
}

void XmlReader::readStartElement(const QString& expectedElementName)
{
    const qint64 line = this->reader.lineNumber();
    const qint64 column = this->reader.columnNumber();

    // Verify token type.
    if (!this->reader.isStartElement())
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
    QString text = this->reader.text().toString();

    // Read end element.
    this->reader.readNext();

    // Advance to next token.
    this->moveToNextToken();

    return text;
}

void XmlReader::moveToNextToken()
{
    this->reader.readNext();

    while (this->reader.isWhitespace())
    {
        this->reader.readNext();
    }
}

void XmlReader::readToken(const QXmlStreamReader::TokenType& expectedTokenType)
{
    const qint64 line = this->reader.lineNumber();
    const qint64 column = this->reader.columnNumber();

    // Verify token type.
    const QXmlStreamReader::TokenType tokenType = this->reader.tokenType();
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
