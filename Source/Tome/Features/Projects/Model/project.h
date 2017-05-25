#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QLocale>

#include "recordidtype.h"
#include "../../Components/Model/componentsetlist.h"
#include "../../Export/Model/recordexporttemplatelist.h"
#include "../../Fields/Model/fielddefinitionsetlist.h"
#include "../../Import/Model/recordtableimporttemplatelist.h"
#include "../../Records/Model/recordsetlist.h"
#include "../../Types/Model/customtypesetlist.h"


namespace Tome
{
    /**
     * @brief Main model of Tome. Sets up and owns all other models.
     */
    class Project
    {
        public:
            /**
             * @brief Constructs a new, empty project.
             */
            Project();

            /**
             * @brief Name of this project, as shown in the UI.
             */
            QString name;

            /**
             * @brief Absolute path to this project, without file name.
             */
            QString path;

            /**
             * @brief Number format of this project.
             */
            QLocale locale;

            /**
             * @brief Whether this project allows to edit read-only data.
             */
            bool ignoreReadOnly;

            /**
             * @brief Type of the ids of the records of this project.
             */
            RecordIdType::RecordIdType recordIdType;

            /**
             * @brief Component sets of this project.
             */
            ComponentSetList componentSets;

            /**
             * @brief Field definition sets of this project.
             */
            FieldDefinitionSetList fieldDefinitionSets;

            /**
             * @brief Record export templates of this project.
             */
            RecordExportTemplateList recordExportTemplates;

            /**
             * @brief Record import templates of this project.
             */
            RecordTableImportTemplateList recordTableImportTemplates;

            /**
             * @brief Record sets of this project.
             */
            RecordSetList recordSets;

            /**
             * @brief Custom type sets of this project.
             */
            CustomTypeSetList typeSets;
    };
}

#endif // PROJECT_H
