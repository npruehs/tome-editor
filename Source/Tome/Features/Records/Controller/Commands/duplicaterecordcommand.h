#ifndef DUPLICATERECORDCOMMAND_H
#define DUPLICATERECORDCOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    /**
     * @brief Duplicates an existing record, including all of its field values.
     */
    class DuplicateRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for duplicating an existing record, including all of its field values.
             * @param recordsController Controller for adding, updating and removing records.
             * @param existingRecordId Id of the record to duplicate.
             * @param newRecordId Id of the record to add.
             */
            DuplicateRecordCommand(RecordsController& recordsController,
                                   const QVariant& existingRecordId,
                                   const QVariant& newRecordId);

            /**
             * @brief Removes the record again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Duplicates the record.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant existingRecordId;
            const QVariant newRecordId;
    };
}

#endif // DUPLICATERECORDCOMMAND_H
