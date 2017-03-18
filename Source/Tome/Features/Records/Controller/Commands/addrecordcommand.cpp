#include "addrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;

AddRecordCommand::AddRecordCommand(RecordsController& recordsController,
                                   const QVariant& id,
                                   const QString& displayName,
                                   const QString& editorIconFieldId,
                                   const QStringList& fieldIds,
                                   const QString& recordSetName,
                                   const QVariant parentId)
    : recordsController(recordsController),
      id(id),
      displayName(displayName),
      editorIconFieldId(editorIconFieldId),
      fieldIds(fieldIds),
      parentId(parentId),
      recordSetName(recordSetName)
{
    this->setText(tr("Add Record - %1").arg(id.toString()));
}

void AddRecordCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo add record %1.").arg(this->id.toString())));

    this->recordsController.removeRecord(this->id);
}

void AddRecordCommand::redo()
{
    const Record& record = this->recordsController.addRecord(
                this->id,
                this->displayName,
                this->editorIconFieldId,
                this->fieldIds,
                this->recordSetName);

    if (!this->parentId.isNull())
    {
        this->recordsController.reparentRecord(record.id, this->parentId);
    }
}
