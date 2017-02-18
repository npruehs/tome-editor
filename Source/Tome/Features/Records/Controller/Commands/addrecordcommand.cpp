#include "addrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;

AddRecordCommand::AddRecordCommand(RecordsController& recordsController,
                                   const QString& id,
                                   const QString& displayName,
                                   const QString& editorIconFieldId,
                                   const QStringList& fieldIds,
                                   const QString& recordSetName,
                                   const QString parentId)
    : recordsController(recordsController),
      id(id),
      displayName(displayName),
      editorIconFieldId(editorIconFieldId),
      fieldIds(fieldIds),
      parentId(parentId),
      recordSetName(recordSetName)
{
    this->setText(tr("Add Record - %1").arg(id));
}

void AddRecordCommand::undo()
{
    qInfo(QString("Undo add record %1.").arg(this->id).toUtf8().constData());

    this->recordsController.removeRecord(this->id);
}

void AddRecordCommand::redo()
{
    this->recordsController.addRecord(
                this->id,
                this->displayName,
                this->editorIconFieldId,
                this->fieldIds,
                this->recordSetName);

    if (!this->parentId.isEmpty())
    {
        this->recordsController.reparentRecord(this->id, this->parentId);
    }
}
