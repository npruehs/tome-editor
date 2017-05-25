#ifndef UPDATERECORDCOMMAND_H
#define UPDATERECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    /**
     * @brief Updates the properties of a record.
     */
    class UpdateRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of a record.
             * @param recordsController Controller for adding, updating and removing records.
             * @param oldId Current id of the record to update.
             * @param newId New id of the record to update.
             * @param newDisplayName New display name of the record to update.
             * @param newEditorIconFieldId New id of the field to use as editor icon for the record to update.
             * @param newFieldIds Ids of the new fields of the record to update.
             * @param newRecordSetName Name of the new record set the record should belong to.
             */
            UpdateRecordCommand(RecordsController& recordsController,
                                const QVariant& oldId,
                                const QVariant& newId,
                                const QString& newDisplayName,
                                const QString& newEditorIconFieldId,
                                const QStringList& newFieldIds,
                                const QString& newRecordSetName);

            /**
             * @brief Reverts all property changes made by this command.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the properties of the record.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant oldId;
            const QVariant newId;
            const QString newDisplayName;
            const QString newEditorIconFieldId;
            const QStringList newFieldIds;
            const QString newRecordSetName;

            QString oldDisplayName;
            QString oldEditorIconFieldId;
            QStringList oldFieldIds;
            QString oldRecordSetName;
    };
}


#endif // UPDATERECORDCOMMAND_H
