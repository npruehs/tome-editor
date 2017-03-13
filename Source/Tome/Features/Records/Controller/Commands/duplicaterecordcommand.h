#ifndef DUPLICATERECORDCOMMAND_H
#define DUPLICATERECORDCOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    class DuplicateRecordCommand : public QUndoCommand, public QObject
    {
        public:
            DuplicateRecordCommand(RecordsController& recordsController,
                                   const QVariant& existingRecordId,
                                   const QVariant& newRecordId);
            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QVariant existingRecordId;
            const QVariant newRecordId;
    };
}

#endif // DUPLICATERECORDCOMMAND_H
