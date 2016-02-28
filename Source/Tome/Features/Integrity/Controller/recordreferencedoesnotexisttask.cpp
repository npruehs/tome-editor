#include "recordreferencedoesnotexisttask.h"

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Records/Model/recordlist.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


RecordReferenceDoesNotExistTask::RecordReferenceDoesNotExistTask()
{
}

const QString Tome::RecordReferenceDoesNotExistTask::getId() const
{
    return "TO0010";
}

const MessageList Tome::RecordReferenceDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all records.
    const RecordList& records = context.recordsController.getRecords();

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        // Get record fields.
        const RecordFieldValueMap& fieldValues = context.recordsController.getRecordFieldValues(record.id);

        // Filter record references.
        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const FieldDefinition& field = context.fieldDefinitionsController.getFieldDefinition(fieldId);

            if (field.fieldType == BuiltInType::Reference)
            {
                // Check record reference.
                const QString& reference = it.value().toString();

                if (!reference.isEmpty() && !context.recordsController.hasRecord(reference))
                {
                    Message message;
                    message.content = "Record reference does not exist: " + reference;
                    message.severity = Severity::Error;
                    message.targetSiteId = record.id;
                    message.targetSiteType = TargetSiteType::Record;

                    messages.append(message);
                }
            }
        }
    }

    return messages;
}
