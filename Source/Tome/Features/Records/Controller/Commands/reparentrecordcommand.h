#ifndef REPARENTRECORDCOMMAND_H
#define REPARENTRECORDCOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    class ReparentRecordCommand : public QUndoCommand, public QObject
    {
        public:
            ReparentRecordCommand(RecordsController& recordsController,
                                  const QVariant& recordId,
                                  const QVariant& newParentId);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant recordId;
            const QVariant newParentId;

            QVariant oldParentId;
    };
}

#endif // REPARENTRECORDCOMMAND_H
