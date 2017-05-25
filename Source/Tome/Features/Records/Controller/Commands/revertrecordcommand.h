#ifndef REVERTRECORDCOMMAND_H
#define REVERTRECORDCOMMAND_H

#include <QUndoCommand>

#include "../../Model/recordfieldvaluemap.h"

namespace Tome
{
    class RecordsController;

    /**
     * @brief Reverts all fields of a record to their inherited or default value.
     */
    class RevertRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for reverting all fields of a record to their inherited or default value.
             * @param recordsController Controller for adding, updating and removing records.
             * @param recordId Id of the record to revert all field values of.
             */
            RevertRecordCommand(RecordsController& recordsController,
                                const QVariant& recordId);

            /**
             * @brief Restores the values of all fields of the record.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Reverts all fields of the record to their inherited or default value.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant recordId;

            RecordFieldValueMap oldRecordFieldValues;
    };
}

#endif // REVERTRECORDCOMMAND_H
