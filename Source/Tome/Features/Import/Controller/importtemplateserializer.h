#ifndef IMPORTTEMPLATESERIALIZER_H
#define IMPORTTEMPLATESERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class RecordTableImportTemplate;

    class ImportTemplateSerializer
    {
        public:
            ImportTemplateSerializer();

            void serialize(QIODevice& device, const RecordTableImportTemplate& importTemplate) const;
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
            static const QString ElementRootRecordId;
            static const QString ElementSourceType;
            static const QString ElementTemplate;

            static const int Version;
    };
}

#endif // IMPORTTEMPLATESERIALIZER_H
