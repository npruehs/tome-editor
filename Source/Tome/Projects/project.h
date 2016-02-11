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
#include "../Types/Model/customtypelist.h"


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
            CustomTypeList types;

            /**
             * @brief getComponentNames Returns the list of the names of all components of this project.
             * @return List containing the names of all components of this project.
             */
            QStringList getComponentNames() const;

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
    };
}

#endif // PROJECT_H
