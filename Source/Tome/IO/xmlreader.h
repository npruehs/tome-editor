#ifndef XMLREADER_H
#define XMLREADER_H

#include <QXmlStreamReader>

/**
 * @brief Reads XML from any device forward-only.
 */
class XmlReader
{
    public:
        /**
         * @brief Constructs a new XML reader for the passed device.
         * @param device Device to read XML from.
         */
        XmlReader(QIODevice *device);
        ~XmlReader();

        /**
         * @brief Gets the name of the current element.
         * @return Name of the current element.
         */
        QString getElementName() const;

        /**
         * @brief Verifies that the reader is at an element with the specified name.
         * @param elementName Expected name of the current element.
         * @return true, if the reader is currently at an element with the specified name, and false otherwise.
         */
        bool isAtElement(const QString& elementName) const;

        /**
         * @brief Reads the value of the attribute with the specified name, without advancing the reader.
         * @param attributeName Name of the attribute to read.
         * @return Value of the attribute of the specified name, or an empty string, if the attribute could not be found.
         */
        QString readAttribute(const QString& attributeName) const;

        /**
         * @brief Verifies that the reader is at an empty element with the specified name, and advances it to the next token.
         *
         * @exception std::runtime_error if the current element isn't an empty element, or doesn't have the specified name.
         *
         * @param elementName Expected name of the current element.
         */
        void readEmptyElement(const QString& elementName);

        /**
         * @brief Verifies that the reader is at the end of the document, and advances it to the very end.
         *
         * @exception std::runtime_error if the current element isn't the end of the document.
         */
        void readEndDocument();

        /**
         * @brief Verifies that the reader is at an end element, and advances the reader to the next token.
         *
         * @exception std::runtime_error if the current element isn't an end element.
         */
        void readEndElement();

        /**
         * @brief Verifies that the reader is at the start of the document, and advances it to the next token.
         *
         * @exception std::runtime_error if the current element isn't the start of the document.
         */
        void readStartDocument();

        /**
         * @brief Verifies that the reader is at a start element with the specified name, and advances it to the next token.
         *
         * @exception std::runtime_error if the current element is not a start element, or doesn't have the specified name.
         *
         * @param expectedElementName Expected name of the current element.
         */
        void readStartElement(const QString& expectedElementName);

        /**
         * @brief Verifies that the reader is at a start element with the specified name, reads its text content, and advances the reader to the next token.
         *
         * @exception std::runtime_error if the current element doesn't have the specified name.
         *
         * @param textElementName Expected name of the current element.
         * @return Text content of the current element.
         */
        QString readTextElement(const QString& textElementName);

        /**
         * @brief Validates the XML document using the specified schema.
         *
         * @exception std::runtime_error if the schema file cannot be opened or is invalid.
         * @exception std::runtime_error if the validation fails.
         *
         * @param schemaFileName Path to the XML schema.
         * @param validationErrorMessage Error message format string, that may contain placeholders for an error message, line and column, in that order.
         */
        void validate(const QString& schemaFileName, const QString& validationErrorMessage);

    private:
        QIODevice* device = nullptr;
        QXmlStreamReader* reader = nullptr;

        void moveToNextToken();
        void readToken(const QXmlStreamReader::TokenType& expectedTokenType);

        void throwTokenError(const qint64& line, const qint64& column) const;
        void throwTokenError(const qint64& line, const qint64& column, const QString& detailMessage) const;
};

#endif // XMLREADER_H
