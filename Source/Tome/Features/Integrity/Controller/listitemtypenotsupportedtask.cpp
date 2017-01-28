#include "listitemtypenotsupportedtask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


const QString ListItemTypeNotSupportedTask::MessageCode = "TO0102";


ListItemTypeNotSupportedTask::ListItemTypeNotSupportedTask()
{
}

const QString ListItemTypeNotSupportedTask::getDisplayName() const
{
    return MessageCode + tr(" - List Item Type Not Supported");
}

const MessageList ListItemTypeNotSupportedTask::execute(const TaskContext& context) const
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
            if (itemType == BuiltInType::Vector2I || itemType == BuiltInType::Vector2R ||
                itemType == BuiltInType::Vector3I || itemType == BuiltInType::Vector3R)
            {
                Message message;
                message.content = tr("List item type %1 is not supported.").arg(itemType);
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

