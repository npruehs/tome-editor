#include "referencedrecorddoesnotexisttask.h"

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Model/builtintype.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


const QString ReferencedRecordDoesNotExistTask::MessageCode = "TO0201";


const QString ReferencedRecordDoesNotExistTask::getDisplayName() const
{
    return MessageCode + tr(" - Referenced Record Does Not Exist");
}

const MessageList ReferencedRecordDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect all records.
    const RecordList& records = context.recordsController.getRecords();

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        for (RecordFieldValueMap::const_iterator it = record.fieldValues.cbegin();
             it != record.fieldValues.cend();
             ++it)
        {
            // Check if is reference.
            const QString& fieldId = it.key();
            const QVariant& fieldValue = it.value();

            const FieldDefinition& field = context.fieldDefinitionsController.getFieldDefinition(fieldId);

            if (!context.typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::Reference))
            {
                continue;
            }

            // Check reference.
            const QString& referencedRecordId = fieldValue.toString();

            if (!referencedRecordId.isEmpty() && !context.recordsController.hasRecord(referencedRecordId))
            {
                Message message;
                message.content = tr("The record %1 referenced by record %2 field %3 does not exist.")
                        .arg(referencedRecordId, record.id.toString(), fieldId);
                message.messageCode = MessageCode;
                message.severity = Severity::Error;
                message.targetSiteId = record.id;
                message.targetSiteType = TargetSiteType::Record;

                messages.append(message);
            }
        }
    }

    return messages;
}
