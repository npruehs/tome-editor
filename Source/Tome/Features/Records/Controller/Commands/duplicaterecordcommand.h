#ifndef DUPLICATERECORDCOMMAND_H
#define DUPLICATERECORDCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class RecordsController;

    class DuplicateRecordCommand : public QUndoCommand
    {
        public:
            DuplicateRecordCommand(RecordsController& recordsController,
                                   const QString& existingRecordId,
                                   const QString& newRecordId);
            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QString existingRecordId;
            const QString newRecordId;
    };
}

#endif // DUPLICATERECORDCOMMAND_H
