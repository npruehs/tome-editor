#include "typefacetviolatedtask.h"

using namespace Tome;

#include "../../Facets/Controller/facetscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"


const QString TypeFacetViolatedTask::MessageCode = "TO0200";


TypeFacetViolatedTask::TypeFacetViolatedTask()
{
}

const QString TypeFacetViolatedTask::getDisplayName() const
{
    return MessageCode + tr(" - Type Facet Violated");
}

const MessageList TypeFacetViolatedTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all records.
    const RecordList& records = context.recordsController.getRecords();

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        // Check all field values.
        const RecordFieldValueMap& fieldValues = context.recordsController.getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            // Validate all facets.
            const QString& fieldId = it.key();
            const QVariant& fieldValue = it.value();

            const FieldDefinition& field = context.fieldDefinitionsController.getFieldDefinition(fieldId);

            const QString& validationError = context.facetsController.validateFieldValue(field.fieldType, fieldValue);

            if (validationError.isEmpty())
            {
                continue;
            }

            Message message;
            message.content = tr("Record %1 field %2 violates a type facet: %3").arg(record.id, fieldId, validationError);
            message.messageCode = MessageCode;
            message.severity = Severity::Error;
            message.targetSiteId = record.id;
            message.targetSiteType = TargetSiteType::Record;

            messages.append(message);
        }
    }

    return messages;
}
