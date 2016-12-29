#ifndef UPDATERECORDCOMMAND_H
#define UPDATERECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class RecordsController;

    class UpdateRecordCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateRecordCommand(RecordsController& recordsController,
                                const QString& oldId,
                                const QString& newId,
                                const QString& newDisplayName,
                                const QStringList& newFieldIds,
                                const QString& newRecordSetName);
            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QString oldId;
            const QString newId;
            const QString newDisplayName;
            const QStringList newFieldIds;
            const QString newRecordSetName;

            QString oldDisplayName;
            QStringList oldFieldIds;
            QString oldRecordSetName;
    };
}


#endif // UPDATERECORDCOMMAND_H
