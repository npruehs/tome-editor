#ifndef REMOVEFIELDDEFINITIONCOMMAND_H
#define REMOVEFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;

    /**
     * @brief Removes a field from the project.
     */
    class RemoveFieldDefinitionCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for removing a field from the project.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param recordsController Controller for adding, updating and removing records.
             * @param id Id of the field to remove.
             */
            RemoveFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                         RecordsController& recordsController,
                                         const QString& id);

            /**
             * @brief Adds the field again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the field.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;

            const QString id;

            QString displayName;
            QString fieldType;
            QVariant defaultValue;
            QString component;
            QString description;
            QString fieldDefinitionSetName;

            QList<QPair<QVariant, QVariant>> removedRecordFieldValues;
    };
}

#endif // REMOVEFIELDDEFINITIONCOMMAND_H
