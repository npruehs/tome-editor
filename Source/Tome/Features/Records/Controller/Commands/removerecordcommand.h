#ifndef REMOVERECORDCOMMAND_H
#define REMOVERECORDCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

#include "../../Model/recordlist.h"

namespace Tome
{
    class RecordsController;
    class FieldDefinitionsController;
    class TypesController;

    /**
     * @brief Removes a record from the project.
     */
    class RemoveRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a command for removing a record from the project.
             * @param recordsController Controller for adding, updating and removing records.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param id Id of the record to remove.
             */
            RemoveRecordCommand(RecordsController& recordsController,
                                FieldDefinitionsController& fieldDefinitionsController,
                                TypesController& typesController,
                                const QVariant& id);

            /**
             * @brief Adds the record again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the record.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;
            FieldDefinitionsController& fieldDefinitionsController;
            TypesController& typesController;

            const QVariant id;

            RecordList removedRecords;
            QList<QPair<QVariant, RecordFieldValueMap>> removedRecordFieldValues;
    };
}

#endif // REMOVERECORDCOMMAND_H
