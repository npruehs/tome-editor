#ifndef ADDRECORDCOMMAND_H
#define ADDRECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    /**
     * @brief Adds a new record to the project.
     */
    class AddRecordCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new record to the project.
             * @param recordsController Controller for adding, updating and removing records.
             * @param id Id of the record to add.
             * @param displayName Display name of the record to add.
             * @param editorIconFieldId Id of the field to use as editor icon for the record to add.
             * @param fieldIds Ids of the fields to add to the new record.
             * @param recordSetName Name of the record set to add the new record to.
             * @param parentId Id of the parent of the record to add.
             */
            AddRecordCommand(RecordsController& recordsController,
                             const QVariant& id,
                             const QString& displayName,
                             const QString& editorIconFieldId,
                             const QStringList& fieldIds,
                             const QString& recordSetName,
                             const QVariant parentId = QVariant());

            /**
             * @brief Removes the record again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the record.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant id;
            const QString displayName;
            const QString editorIconFieldId;
            const QStringList fieldIds;
            const QVariant parentId;
            const QString recordSetName;
    };
}

#endif // ADDRECORDCOMMAND_H
