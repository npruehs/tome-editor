#ifndef REMOVEFIELDDEFINITIONCOMMAND_H
#define REMOVEFIELDDEFINITIONCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;

    class RemoveFieldDefinitionCommand : public QUndoCommand, public QObject
    {
        public:
            RemoveFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                         RecordsController& recordsController,
                                         const QString& id);

            virtual void undo();
            virtual void redo();

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

            QVariantMap removedRecordFieldValues;
    };
}

#endif // REMOVEFIELDDEFINITIONCOMMAND_H
