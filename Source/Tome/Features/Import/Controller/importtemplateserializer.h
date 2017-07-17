#ifndef IMPORTTEMPLATESERIALIZER_H
#define IMPORTTEMPLATESERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class RecordTableImportTemplate;

    /**
     * @brief Reads and writes record import templates from any device.
     */
    class ImportTemplateSerializer
    {
        public:
            /**
             * @brief serialize Writes the passed record import template to the specified device.
             * @param device Device to write the record import template to.
             * @param importTemplate Record import templatet to write.
             */
            void serialize(QIODevice& device, const RecordTableImportTemplate& importTemplate) const;

            /**
             * @brief deserialize Reads the record import template from the specified device.
             * @param device Device to read the record import template from.
             * @param importTemplate Record import template to read the data into.
             */
            void deserialize(QIODevice& device, RecordTableImportTemplate& importTemplate) const;

        private:
            static const QString AttributeColumnName;
            static const QString AttributeFieldId;
            static const QString AttributeKey;
            static const QString AttributeValue;
            static const QString AttributeVersion;

            static const QString ElementColumnMap;
            static const QString ElementId;
            static const QString ElementIdColumn;
            static const QString ElementIgnoredIds;
            static const QString ElementMapping;
            static const QString ElementName;
            static const QString ElementParameter;
            static const QString ElementParameters;
            static const QString ElementReplaceWith;
            static const QString ElementRootRecordId;
            static const QString ElementSourceType;
            static const QString ElementString;
            static const QString ElementStringReplacementMap;
            static const QString ElementTemplate;

            static const int Version;
    };
}

#endif // IMPORTTEMPLATESERIALIZER_H
