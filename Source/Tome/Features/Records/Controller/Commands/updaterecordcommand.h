#ifndef UPDATERECORDCOMMAND_H
#define UPDATERECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    class UpdateRecordCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateRecordCommand(RecordsController& recordsController,
                                const QVariant& oldId,
                                const QVariant& newId,
                                const QString& newDisplayName,
                                const QString& newEditorIconFieldId,
                                const QStringList& newFieldIds,
                                const QString& newRecordSetName);
            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            RecordsController& recordsController;

            const QVariant oldId;
            const QVariant newId;
            const QString newDisplayName;
            const QString newEditorIconFieldId;
            const QStringList newFieldIds;
            const QString newRecordSetName;

            QString oldDisplayName;
            QString oldEditorIconFieldId;
            QStringList oldFieldIds;
            QString oldRecordSetName;
    };
}


#endif // UPDATERECORDCOMMAND_H
