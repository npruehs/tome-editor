#include "listitemtypedoesnotexisttask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


const QString ListItemTypeDoesNotExistTask::MessageCode = "TO0101";


const QString ListItemTypeDoesNotExistTask::getDisplayName() const
{
    return MessageCode + tr(" - List Item Type Does Not Exist");
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
                message.content = tr("The item type %1 of the list type %2 does not exist.").arg(itemType, type.name);
                message.messageCode = MessageCode;
                message.severity = Severity::Error;
                message.targetSiteId = type.name;
                message.targetSiteType = TargetSiteType::Type;

                messages.append(message);
            }
        }
    }

    return messages;
}

