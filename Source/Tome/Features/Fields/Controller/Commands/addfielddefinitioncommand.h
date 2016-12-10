#ifndef ADDFIELDDEFINITIONCOMMAND_H
#define ADDFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;

    class AddFieldDefinitionCommand : public QUndoCommand, public QObject
    {
        public:
            AddFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                      const QString& id,
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
