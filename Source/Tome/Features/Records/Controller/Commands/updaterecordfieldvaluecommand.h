#ifndef UPDATERECORDFIELDVALUECOMMAND_H
#define UPDATERECORDFIELDVALUECOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    /**
     * @brief Updates the current value of a record field.
     */
    class UpdateRecordFieldValueCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the current value of a record field.
             * @param recordsController Controller for adding, updating and removing records.
             * @param recordId Id of the record to update the field value of.
             * @param fieldId Id of the field to update the value of.
             * @param newFieldValue New value of the field to update.
             */
            UpdateRecordFieldValueCommand(RecordsController& recordsController,
                                          const QVariant& recordId,
                                          const QString& fieldId,
                                          const QVariant& newFieldValue);

            /**
             * @brief Restores the previous field value.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the field value.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant recordId;
            const QString fieldId;
            const QVariant newFieldValue;

            QVariant oldFieldValue;
    };
}

#endif // UPDATERECORDFIELDVALUECOMMAND_H
