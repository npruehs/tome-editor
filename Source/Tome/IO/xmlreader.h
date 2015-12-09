#ifndef XMLREADER_H
#define XMLREADER_H

#include <QSharedPointer>
#include <QXmlStreamReader>

class XmlReader
{
    public:
        XmlReader(QSharedPointer<QXmlStreamReader> reader);

        /**
         * @brief getElementName Gets the name of the current element.
         * @return Name of the current element.
         */
        QString getElementName();

        /**
         * @brief isAtElement Verifies that the reader is at an element with the specified name.
         * @param elementName Expected name of the current element.
         * @return true, if the reader is currently at an element with the specified name, and false otherwise.
         */
        bool isAtElement(const QString& elementName);

        /**
         * @brief readEndDocument Verifies that the reader is at the end of the document, and advances it to the very end.
         */
        void readEndDocument();

        /**
         * @brief readEndElement Verifies that the reader is at an end element, and advances the reader to the next token.
         */
        void readEndElement();

        /**
         * @brief readStartDocument Verifies that the reader is at the start of the document, and advances it to the next token.
         */
        void readStartDocument();

        /**
         * @brief readStartElement Verifies that the reader is at a start element with the specified name, and advances it to the next token.
         * @param expectedElementName Expected name of the current element.
         */
        void readStartElement(const QString& expectedElementName);

        /**
         * @brief readTextElement Verifies that the reader is at a start element with the specified name, reads its text content, and advances the reader to the next token.
         * @param textElementName Expected name of the current element.
         * @return Text content of the current element.
         */
        QString readTextElement(const QString& textElementName);

    private:
        QSharedPointer<QXmlStreamReader> reader;

        void moveToNextToken();
        void readToken(const QXmlStreamReader::TokenType& expectedTokenType);

        void throwTokenError(const qint64& line, const qint64& column);
        void throwTokenError(const qint64& line, const qint64& column, const QString& detailMessage);
};

#endif // XMLREADER_H
