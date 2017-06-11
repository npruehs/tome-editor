#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

namespace Tome
{
    class Project;

    /**
     * @brief Reads and writes projects from any device.
     */
    class ProjectSerializer
    {
        public:
            /**
             * @brief Writes the passed project to the specified device.
             * @param device Device to write the project to.
             * @param project Project to write.
             */
            void serialize(QIODevice& device, QSharedPointer<Project> project) const;

            /**
             * @brief Reads the project from the specified device.
             * @param device Device to read the project from.
             * @param project Project to fill with read data.
             */
            void deserialize(QIODevice& device, QSharedPointer<Project> project) const;

        private:
            static const QString AttributeBaseType;
            static const QString AttributeExportedType;
            static const QString AttributeExportAsTable;
            static const QString AttributeExportRoots;
            static const QString AttributeExportInnerNodes;
            static const QString AttributeExportLeafs;
            static const QString AttributeIgnoreReadOnly;
            static const QString AttributeKey;
            static const QString AttributeRecordIdType;
            static const QString AttributeTomeType;
            static const QString AttributeValue;
            static const QString AttributeVersion;
            static const QString ElementComponents;
            static const QString ElementFieldDefinitions;
            static const QString ElementFileExtension;
            static const QString ElementMapping;
            static const QString ElementName;
            static const QString ElementLocale;
            static const QString ElementPath;
            static const QString ElementRecords;
            static const QString ElementRecordExportTemplates;
            static const QString ElementRecordImportTemplates;
            static const QString ElementRestriction;
            static const QString ElementRestrictions;
            static const QString ElementTemplate;
            static const QString ElementTomeProject;
            static const QString ElementType;
            static const QString ElementTypes;
            static const QString ElementTypeMap;

            static const int Version;
    };
}

#endif // PROJECTSERIALIZER_H
