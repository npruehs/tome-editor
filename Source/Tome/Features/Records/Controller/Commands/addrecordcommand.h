#ifndef ADDRECORDCOMMAND_H
#define ADDRECORDCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class RecordsController;

    class AddRecordCommand : public QUndoCommand, public QObject
    {
        public:
            AddRecordCommand(RecordsController& recordsController,
                             const QString& id,
                             const QString& displayName,
                             const QStringList& fieldIds,
                             const QString& recordSetName,
                             const QString parentId = QString());
            virtual void undo();
            virtual void redo();

        private:
            RecordsController& recordsController;

            const QString id;
            const QString displayName;
            const QStringList fieldIds;
            const QString parentId;
            const QString recordSetName;
    };
}

#endif // ADDRECORDCOMMAND_H
