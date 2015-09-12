#ifndef FIELDDEFINITIONFILE_H
#define FIELDDEFINITIONFILE_H

#include <list>
#include <QSharedPointer>

#include "fielddefinition.h"

#define PROJECT_FILE_EXTENSION ".tfield"

namespace Tome
{
    class FieldDefinitionFile
    {
    public:
        FieldDefinitionFile();

        QString path;
        std::list< QSharedPointer<FieldDefinition> > fieldDefinitions;
    };
}

#endif // FIELDDEFINITIONFILE_H
