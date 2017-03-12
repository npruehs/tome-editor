#ifndef ADDRECORDCOMMAND_H
#define ADDRECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>
#include <QVariant>

namespace Tome
{
    class RecordsController;

    class AddRecordCommand : public QUndoCommand, public QObject
    {
        public:
            AddRecordCommand(RecordsController& recordsController,
                             const QVariant& id,
                             const QString& displayName,
                             const QString& editorIconFieldId,
                             const QStringList& fieldIds,
                             const QString& recordSetName,
                             const QVariant parentId = QVariant());
            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QVariant id;
            const QString displayName;
            const QString editorIconFieldId;
            const QStringList fieldIds;
            const QVariant parentId;
            const QString recordSetName;
    };
}

#endif // ADDRECORDCOMMAND_H
