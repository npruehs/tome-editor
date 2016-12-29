#ifndef EXPORTTEMPLATESERIALIZER_H
#define EXPORTTEMPLATESERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class RecordExportTemplate;

    class ExportTemplateSerializer
    {
        public:
            ExportTemplateSerializer();

            void serialize(QIODevice& device, const RecordExportTemplate& exportTemplate) const;
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
            static const QString ElementMapping;
            static const QString ElementName;
            static const QString ElementTemplate;
            static const QString ElementTypeMap;

            static const int Version;
    };
}

#endif // EXPORTTEMPLATESERIALIZER_H
