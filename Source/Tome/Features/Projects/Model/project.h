#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QLocale>

#include "../../Components/Model/componentsetlist.h"
#include "../../Export/Model/recordexporttemplatemap.h"
#include "../../Fields/Model/fielddefinitionsetlist.h"
#include "../../Records/Model/recordsetlist.h"
#include "../../Types/Model/customtypelist.h"


namespace Tome
{
    class Project
    {
        public:
            Project();

            QString name;
            QString path;
            QLocale locale;

            ComponentSetList componentSets;
            FieldDefinitionSetList fieldDefinitionSets;
            RecordExportTemplateMap recordExportTemplates;
            RecordSetList recordSets;
            CustomTypeList types;
    };
}

#endif // PROJECT_H
