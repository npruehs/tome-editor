#ifndef PROJECT_H
#define PROJECT_H

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVector>

#include "../Components/Model/componentlist.h"
#include "../Export/Model/recordexporttemplate.h"
#include "../Fields/Model/fielddefinitionsetlist.h"
#include "../Records/Model/recordsetlist.h"
#include "../Types/customtype.h"


namespace Tome
{
    class Project
    {
        public:
            Project();

            QString name;
            QString path;

            ComponentList components;
            FieldDefinitionSetList fieldDefinitionSets;
            QMap<QString, QSharedPointer<RecordExportTemplate> > recordExportTemplates;
            RecordSetList recordSets;
            QVector<QSharedPointer<CustomType> > types;

            /**
             * @brief getComponentNames Returns the list of the names of all components of this project.
             * @return List containing the names of all components of this project.
             */
            QStringList getComponentNames() const;

            /**
             * @brief getCustomType Returns the custom type with the specified name, of a null pointer if that type could not be found.
             * @param name Name of the type to get.
             * @return Custom type with the specified name, of a null pointer if that type could not be found.
             */
            QSharedPointer<CustomType> getCustomType(const QString& name) const;

            /**
             * @brief getRecordByDisplayName Returns the record with the specified id, or a null pointer if that record could not be found.
             * @param id Id of the record to get.
             * @return Record with the specified id, or a null pointer if that record could not be found.
             */
            Record& getRecord(const QString& id);

            /**
             * @brief getRecordByDisplayName Returns the record with the specified display name, or a null pointer if that record could not be found.
             * @param displayName Display name of the record to get.
             * @return Record with the specified display name, or a null pointer if that record could not be found.
             */
            Record& getRecordByDisplayName(const QString& displayName);

            /**
             * @brief getRecordNames Returns the list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            QStringList getRecordNames() const;

            /**
             * @brief getTypeNames Returns a list of all type names of this project, including built-in types.
             * @return List containing the names of all types avaialable in this project.
             */
            QStringList getTypeNames() const;
    };
}

#endif // PROJECT_H
