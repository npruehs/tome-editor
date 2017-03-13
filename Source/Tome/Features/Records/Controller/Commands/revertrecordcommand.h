#ifndef REVERTRECORDCOMMAND_H
#define REVERTRECORDCOMMAND_H

#include <QUndoCommand>

#include "../../Model/recordfieldvaluemap.h"

namespace Tome
{
    class RecordsController;

    class RevertRecordCommand : public QUndoCommand, public QObject
    {
        public:
            RevertRecordCommand(RecordsController& recordsController,
                                const QVariant& recordId);

            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QVariant recordId;

            RecordFieldValueMap oldRecordFieldValues;
    };
}

#endif // REVERTRECORDCOMMAND_H
