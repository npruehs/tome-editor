#ifndef REMOVERECORDCOMMAND_H
#define REMOVERECORDCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariant>

#include "../../Model/recordlist.h"

namespace Tome
{
    class RecordsController;
    class FieldDefinitionsController;
    class TypesController;

    class RemoveRecordCommand : public QUndoCommand, public QObject
    {
        public:
            RemoveRecordCommand(RecordsController& recordsController,
                                FieldDefinitionsController& fieldDefinitionsController,
                                TypesController& typesController,
                                const QVariant& id);

            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;
            FieldDefinitionsController& fieldDefinitionsController;
            TypesController& typesController;

            const QVariant id;

            RecordList removedRecords;
            QMap<QVariant, RecordFieldValueMap> removedRecordFieldValues;
    };
}

#endif // REMOVERECORDCOMMAND_H
