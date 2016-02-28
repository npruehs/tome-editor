#include "listitemtypedoesnotexisttask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


ListItemTypeDoesNotExistTask::ListItemTypeDoesNotExistTask()
{
}

const QString ListItemTypeDoesNotExistTask::getId() const
{
    return "TO0011";
}

const MessageList ListItemTypeDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all types.
    const CustomTypeList& types = context.typesController.getCustomTypes();

    for (int i = 0; i < types.count(); ++i)
    {
        const CustomType& type = types.at(i);

        if (type.isList())
        {
            const QString itemType = type.getItemType();

            // Check list item type.
            if (!context.typesController.isBuiltInType(itemType) && !context.typesController.isCustomType(itemType))
            {
                Message message;
                message.content = "List item type " + itemType + " does not exist.";
                message.severity = Severity::Error;
                message.targetSiteId = type.name;
                message.targetSiteType = TargetSiteType::Type;

                messages.append(message);
            }
        }
    }

    return messages;
}

