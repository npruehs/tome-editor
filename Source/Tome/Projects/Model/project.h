#ifndef PROJECT_H
#define PROJECT_H

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVector>

#include "../../Components/Model/componentlist.h"
#include "../../Export/Model/recordexporttemplate.h"
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

            ComponentList components;
            FieldDefinitionSetList fieldDefinitionSets;
            QMap<QString, QSharedPointer<RecordExportTemplate> > recordExportTemplates;
            RecordSetList recordSets;
            CustomTypeList types;
    };
}

#endif // PROJECT_H
