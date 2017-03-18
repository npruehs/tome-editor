#ifndef UPDATERECORDFIELDVALUECOMMAND_H
#define UPDATERECORDFIELDVALUECOMMAND_H

#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    class UpdateRecordFieldValueCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateRecordFieldValueCommand(RecordsController& recordsController,
                                          const QVariant& recordId,
                                          const QString& fieldId,
                                          const QVariant& newFieldValue);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant recordId;
            const QString fieldId;
            const QVariant newFieldValue;

            QVariant oldFieldValue;
    };
}

#endif // UPDATERECORDFIELDVALUECOMMAND_H
