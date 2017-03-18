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

            virtual void undo() Q_DECL_OVERRIDE;
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

            QMap<QVariant, QVariant> removedRecordFieldValues;
    };
}

#endif // REMOVEFIELDDEFINITIONCOMMAND_H
