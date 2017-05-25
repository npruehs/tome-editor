#ifndef ADDFIELDDEFINITIONCOMMAND_H
#define ADDFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;

    /**
     * @brief Adds a new field to the project.
     */
    class AddFieldDefinitionCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new field to the project.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param id Id of the field to add.
             * @param displayName Display name of the field to add.
             * @param fieldType Type of the field to add.
             * @param defaultValue Default value of the field to add.
             * @param component Component the field to add belongs to.
             * @param description Textual description of the field to add.
             * @param fieldDefinitionSetName Name of the field definition set to add the new field to.
             */
            AddFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                      const QString& id,
                                      const QString& displayName,
                                      const QString& fieldType,
                                      const QVariant& defaultValue,
                                      const QString& component,
                                      const QString& description,
                                      const QString& fieldDefinitionSetName);

            /**
             * @brief Removes the field again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the field.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            FieldDefinitionsController& fieldDefinitionsController;

            const QString id;
            const QString displayName;
            const QString fieldType;
            const QVariant defaultValue;
            const QString component;
            const QString description;
            const QString fieldDefinitionSetName;
    };
}

#endif // ADDFIELDDEFINITIONCOMMAND_H
