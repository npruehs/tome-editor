#ifndef FINDUSAGESCONTROLLER_H
#define FINDUSAGESCONTROLLER_H

#include <QObject>

#include "../Model/searchresultlist.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Controller for finding usages of fields, records and custom types.
     */
    class FindUsagesController : public QObject
    {
        Q_OBJECT

        public:
            /**
             * @brief Constructs a new controller for finding usages of fields, records and custom types.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            FindUsagesController(const FieldDefinitionsController& fieldDefinitionsController,
                                 const RecordsController& recordsController,
                                 const TypesController& typesController);

            /**
             * @brief Searches for records who are using the field with the specified id.
             * @param fieldId Id of the field to search for.
             * @return Records who are using the field with the specified id.
             */
            const SearchResultList findUsagesOfField(const QString& fieldId) const;

            /**
             * @brief Searches for records whose field values are referencing the record with the specified id.
             * @param recordId Id of the record to find all references of.
             * @return Records whose field values are referencing the record with the specified id.
             */
            const SearchResultList findUsagesOfRecord(const QVariant& recordId) const;

            /**
             * @brief Searches for records whose fields are of the type with the specified name.
             * @param typeName Name of the type to find all usages of.
             * @return Records whose fields are of the type with the specified name.
             */
            const SearchResultList findUsagesOfType(const QString& typeName) const;

        signals:
            /**
             * @brief Progress of the current search operation has changed.
             * @param title Title of the current search operation step.
             * @param text Details of the current search operation step.
             * @param currentValue Current progress value of the current search operation step.
             * @param maximumValue Maximum progress value of the current search operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

            /**
             * @brief A search has been completed.
             * @param title Title of the search operation.
             * @param results Results of the search operation.
             */
            void searchResultChanged(const QString& title, const Tome::SearchResultList results) const;

        private:
            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // FINDUSAGESCONTROLLER_H
