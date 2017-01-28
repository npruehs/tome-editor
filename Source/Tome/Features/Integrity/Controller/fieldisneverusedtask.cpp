#include "fieldisneverusedtask.h"

#include <QSet>

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"

using namespace Tome;


const QString FieldIsNeverUsedTask::MessageCode = "TO0301";


FieldIsNeverUsedTask::FieldIsNeverUsedTask()
{
}

const QString FieldIsNeverUsedTask::getDisplayName() const
{
    return MessageCode + tr(" - Field Is Never Used");
}

const MessageList FieldIsNeverUsedTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect all used fields.
    const RecordList& records = context.recordsController.getRecords();

    QSet<QString> usedFields;

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        for (RecordFieldValueMap::const_iterator it = record.fieldValues.begin();
             it != record.fieldValues.end();
             ++it)
        {
            usedFields.insert(it.key());
        }
    }

    // Check all field usage.
    const FieldDefinitionList& fields = context.fieldDefinitionsController.getFieldDefinitions();

    for (int i = 0; i < fields.count(); ++i)
    {
        const FieldDefinition& field = fields.at(i);

        if (!usedFields.contains(field.id))
        {
            Message message;
            message.content = tr("The field %1 is defined but never used.").arg(field.id);
            message.messageCode = MessageCode;
            message.severity = Severity::Warning;
            message.targetSiteId = field.id;
            message.targetSiteType = TargetSiteType::Field;

            messages.append(message);
        }
    }

    return messages;
}
