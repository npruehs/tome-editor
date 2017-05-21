#ifndef UPDATEFIELDDEFINITIONCOMMAND_H
#define UPDATEFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;

    /**
     * @brief Updates the properties of a field definition.
     */
    class UpdateFieldDefinitionCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of a field definition.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param oldId Current id of the field to update.
             * @param newId New id of the field to update.
             * @param displayName New display name of the field.
             * @param fieldType New type of the field.
             * @param defaultValue New default value of the field.
             * @param component New component the field should belong to.
             * @param description New textual description of the field.
             * @param fieldDefinitionSetName Name of the new field definition set the field should belong to.
             */
            UpdateFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                         const QString& oldId,
                                         const QString& newId,
                                         const QString& displayName,
                                         const QString& fieldType,
                                         const QVariant& defaultValue,
                                         const QString& component,
                                         const QString& description,
                                         const QString& fieldDefinitionSetName);

            /**
             * @brief Reverts all property changes made by this command.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the properties of the field.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            FieldDefinitionsController& fieldDefinitionsController;

            const QString oldId;
            const QString newId;
            const QString newDisplayName;
            const QString newFieldType;
            const QVariant newDefaultValue;
            const QString newComponent;
            const QString newDescription;
            const QString newFieldDefinitionSetName;

            QString oldDisplayName;
            QString oldFieldType;
            QVariant oldDefaultValue;
            QString oldComponent;
            QString oldDescription;
            QString oldFieldDefinitionSetName;
    };
}

#endif // UPDATEFIELDDEFINITIONCOMMAND_H
