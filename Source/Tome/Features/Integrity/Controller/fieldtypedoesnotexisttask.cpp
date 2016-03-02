#include "fieldtypedoesnotexisttask.h"

using namespace Tome;

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"


const QString FieldTypeDoesNotExistTask::MessageCode = "TO0100";


FieldTypeDoesNotExistTask::FieldTypeDoesNotExistTask()
{
}

const MessageList FieldTypeDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all fields.
    const FieldDefinitionList& fields = context.fieldDefinitionsController.getFieldDefinitions();

    for (int i = 0; i < fields.count(); ++i)
    {
        const FieldDefinition& field = fields.at(i);

        // Check field type.
        if (!context.typesController.isBuiltInType(field.fieldType) && !context.typesController.isCustomType(field.fieldType))
        {
            Message message;
            message.content = "Field type " + field.fieldType + " does not exist.";
            message.messageCode = MessageCode;
            message.severity = Severity::Error;
            message.targetSiteId = field.id;
            message.targetSiteType = TargetSiteType::Field;

            messages.append(message);
        }
    }

    return messages;
}
