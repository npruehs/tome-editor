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

            const SearchResultList findUsagesOfField(const QString& fieldId) const;
            const SearchResultList findUsagesOfRecord(const QVariant& recordId) const;
            const SearchResultList findUsagesOfType(const QString& typeName) const;

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void searchResultChanged(const QString& title, const Tome::SearchResultList results) const;

        private:
            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // FINDUSAGESCONTROLLER_H
