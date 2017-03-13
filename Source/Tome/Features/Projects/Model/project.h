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
    class Project
    {
        public:
            Project();

            QString name;
            QString path;
            QLocale locale;
            bool ignoreReadOnly;
            RecordIdType::RecordIdType recordIdType;

            ComponentSetList componentSets;
            FieldDefinitionSetList fieldDefinitionSets;
            RecordExportTemplateList recordExportTemplates;
            RecordTableImportTemplateList recordTableImportTemplates;
            RecordSetList recordSets;
            CustomTypeSetList typeSets;
    };
}

#endif // PROJECT_H
