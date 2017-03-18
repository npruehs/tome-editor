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

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

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
