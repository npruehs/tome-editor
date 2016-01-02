#ifndef PROJECT_H
#define PROJECT_H

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVector>

#include "../Export/recordexporttemplate.h"
#include "../Fields/fielddefinitionset.h"
#include "../Records/recordset.h"
#include "../Types/customtype.h"


namespace Tome
{
    class Project
    {
        public:
            Project();

            QString name;
            QString path;

            QVector<QString> components;
            QVector< QSharedPointer<FieldDefinitionSet> > fieldDefinitionSets;
            QMap<QString, QSharedPointer<RecordExportTemplate> > recordExportTemplates;
            QVector< QSharedPointer<RecordSet> > recordSets;
            QVector<QSharedPointer<CustomType> > types;

            /**
             * @brief getComponentNames Returns the list of the names of all components of this project.
             * @return List containing the names of all components of this project.
             */
            QStringList getComponentNames() const;

            /**
             * @brief getFieldDefinition Returns the field definition with the specified id, or a null pointer if that field could not be found.
             * @param id Id of the field definition to get.
             * @return Field definition with the specified id, or a null pointer if that field could not be found.
             */
            QSharedPointer<FieldDefinition> getFieldDefinition(const QString& id) const;

            /**
             * @brief getRecordByDisplayName Returns the record with the specified id, or a null pointer if that record could not be found.
             * @param id Id of the record to get.
             * @return Record with the specified id, or a null pointer if that record could not be found.
             */
            QSharedPointer<Record> getRecord(const QString& id) const;

            /**
             * @brief getRecordByDisplayName Returns the record with the specified display name, or a null pointer if that record could not be found.
             * @param displayName Display name of the record to get.
             * @return Record with the specified display name, or a null pointer if that record could not be found.
             */
            QSharedPointer<Record> getRecordByDisplayName(const QString& displayName) const;

            /**
             * @brief getRecordNames Returns the list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            QStringList getRecordNames() const;
    };
}

#endif // PROJECT_H
