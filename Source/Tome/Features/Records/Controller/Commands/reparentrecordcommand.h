#ifndef REPARENTRECORDCOMMAND_H
#define REPARENTRECORDCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class RecordsController;

    class ReparentRecordCommand : public QUndoCommand
    {
        public:
            ReparentRecordCommand(RecordsController& recordsController,
                                  const QString& recordId,
                                  const QString& newParentId);

            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QString recordId;
            const QString newParentId;

            QString oldParentId;
    };
}

#endif // REPARENTRECORDCOMMAND_H
