#ifndef TOMEPROJECT_H
#define TOMEPROJECT_H

#include <list>

#include <QSharedPointer>
#include <QString>

#include "Fields/fielddefinitionfile.h"

namespace Tome
{
    class TomeProject
    {
    public:
        TomeProject();

        QString name;
        std::list< QSharedPointer<FieldDefinitionFile> > fieldDefinitionFiles;
    };
}

#endif // TOMEPROJECT_H
