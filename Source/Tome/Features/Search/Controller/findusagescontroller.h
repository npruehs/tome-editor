#ifndef FINDUSAGESCONTROLLER_H
#define FINDUSAGESCONTROLLER_H

#include <QObject>

#include "../Model/searchresultlist.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class FindUsagesController : public QObject
    {
        Q_OBJECT

        public:
            FindUsagesController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController);

            const SearchResultList findUsagesOfType(const QString& typeName);

        signals:
            void searchResultChanged(const QString& title, const Tome::SearchResultList results);

        private:
            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // FINDUSAGESCONTROLLER_H
