#ifndef UPDATEFIELDDEFINITIONCOMMAND_H
#define UPDATEFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;

    class UpdateFieldDefinitionCommand : public QUndoCommand
    {
        public:
            UpdateFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                         const QString& oldId, const QString& newId,
                                         const QString& displayName,
                                         const QString& fieldType,
                                         const QVariant& defaultValue,
                                         const QString& component,
                                         const QString& description,
                                         const QString& fieldDefinitionSetName);

            virtual void undo();
            virtual void redo();

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
