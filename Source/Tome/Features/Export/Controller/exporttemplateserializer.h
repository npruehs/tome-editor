#ifndef EXPORTTEMPLATESERIALIZER_H
#define EXPORTTEMPLATESERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class RecordExportTemplate;

    /**
     * @brief Reads and writes record export templates from any device.
     */
    class ExportTemplateSerializer
    {
        public:
            /**
             * @brief Constructs a new serializer for reading and writing record export templates from any device.
             */
            ExportTemplateSerializer();

            /**
             * @brief Writes the passed record export template to the specified device.
             * @param device Device to write the record export template to.
             * @param exportTemplate Record export template to write.
             */
            void serialize(QIODevice& device, const RecordExportTemplate& exportTemplate) const;

            /**
             * @brief Reads the record export template from the specified device.
             * @param device Device to read the record export template from.
             * @param exportTemplate Record export template to read the data into.
             */
            void deserialize(QIODevice& device, RecordExportTemplate& exportTemplate) const;

        private:
            static const QString AttributeExportAsTable;
            static const QString AttributeExportRoots;
            static const QString AttributeExportInnerNodes;
            static const QString AttributeExportLeafs;
            static const QString AttributeExportLocalizedFieldsOnly;
            static const QString AttributeExportedType;
            static const QString AttributeTomeType;
            static const QString AttributeVersion;
            static const QString ElementFileExtension;
            static const QString ElementId;
            static const QString ElementIgnoredFields;
            static const QString ElementIgnoredRecords;
            static const QString ElementIncludedRecords;
            static const QString ElementMapping;
            static const QString ElementName;
            static const QString ElementReplaceWith;
            static const QString ElementString;
            static const QString ElementStringReplacementMap;
            static const QString ElementTemplate;
            static const QString ElementTypeMap;

            static const int Version;
    };
}

#endif // EXPORTTEMPLATESERIALIZER_H
