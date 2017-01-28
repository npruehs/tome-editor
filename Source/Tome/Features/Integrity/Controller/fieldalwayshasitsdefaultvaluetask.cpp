#include "fieldalwayshasitsdefaultvaluetask.h"

#include <QSet>

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"

using namespace Tome;


const QString FieldAlwaysHasItsDefaultValueTask::MessageCode = "TO0303";


FieldAlwaysHasItsDefaultValueTask::FieldAlwaysHasItsDefaultValueTask()
{
}

const QString FieldAlwaysHasItsDefaultValueTask::getDisplayName() const
{
    return MessageCode + tr(" - Field Always Has Its Default Value");
}

const MessageList FieldAlwaysHasItsDefaultValueTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect non-default field values.
    const RecordList& records = context.recordsController.getRecords();

    QSet<QString> nonDefaultFieldValues;

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        for (RecordFieldValueMap::const_iterator it = record.fieldValues.begin();
             it != record.fieldValues.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const QVariant& fieldValue = it.value();

            const FieldDefinition& field = context.fieldDefinitionsController.getFieldDefinition(fieldId);

            if (field.defaultValue != fieldValue)
            {
                nonDefaultFieldValues.insert(fieldId);
            }
        }
    }

    // Check all field usage.
    const FieldDefinitionList& fields = context.fieldDefinitionsController.getFieldDefinitions();

    for (int i = 0; i < fields.count(); ++i)
    {
        const FieldDefinition& field = fields.at(i);

        if (!nonDefaultFieldValues.contains(field.id))
        {
            Message message;
            message.content = tr("The field %1 is never assigned and always has its default value.").arg(field.id);
            message.messageCode = MessageCode;
            message.severity = Severity::Warning;
            message.targetSiteId = field.id;
            message.targetSiteType = TargetSiteType::Field;

            messages.append(message);
        }
    }

    return messages;
}
