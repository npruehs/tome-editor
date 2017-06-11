#ifndef REPARENTRECORDCOMMAND_H
#define REPARENTRECORDCOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    /**
     * @brief Changes the parent of an existing record.
     */
    class ReparentRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for changing the parent of an existing record.
             * @param recordsController Controller for adding, updating and removing records.
             * @param recordId Id of the record to change the parent of.
             * @param newParentId Id of the new parent of the record.
             */
            ReparentRecordCommand(RecordsController& recordsController,
                                  const QVariant& recordId,
                                  const QVariant& newParentId);

            /**
             * @brief Restores the previous parent of the record.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Changes the parent of the record.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant recordId;
            const QVariant newParentId;

            QVariant oldParentId;
    };
}

#endif // REPARENTRECORDCOMMAND_H
