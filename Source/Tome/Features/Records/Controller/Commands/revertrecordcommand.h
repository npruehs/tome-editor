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
                                const QString& recordId);

            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QString recordId;

            RecordFieldValueMap oldRecordFieldValues;
    };
}

#endif // REVERTRECORDCOMMAND_H
