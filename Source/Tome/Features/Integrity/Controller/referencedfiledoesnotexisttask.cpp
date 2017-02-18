#include "referencedfiledoesnotexisttask.h"

#include <QFileInfo>

#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Controller/removedfileprefixfacet.h"
#include "../../Facets/Controller/removedfilesuffixfacet.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Projects/Controller/projectcontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Model/builtintype.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../../Util/pathutils.h"


using namespace Tome;


const QString ReferencedFileDoesNotExistTask::MessageCode = "TO0202";


ReferencedFileDoesNotExistTask::ReferencedFileDoesNotExistTask()
{
}

const QString ReferencedFileDoesNotExistTask::getDisplayName() const
{
    return MessageCode + tr(" - Referenced File Does Not Exist");
}

const MessageList ReferencedFileDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect all records.
    const RecordList& records = context.recordsController.getRecords();

    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        for (RecordFieldValueMap::const_iterator it = record.fieldValues.begin();
             it != record.fieldValues.end();
             ++it)
        {
            // Check if is file.
            const QString& fieldId = it.key();
            const QVariant& fieldValue = it.value();

            const FieldDefinition& field = context.fieldDefinitionsController.getFieldDefinition(fieldId);

            if (!context.typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::File))
            {
                continue;
            }

            QString fileName = fieldValue.toString();

            if (fileName.isEmpty())
            {
                continue;
            }

            // Get removed file prefix and suffix.
            QString removedPrefix;
            QString removedSuffix;

            removedPrefix = context.facetsController.getFacetValue(field.fieldType, RemovedFilePrefixFacet::FacetKey).toString();
            removedSuffix = context.facetsController.getFacetValue(field.fieldType, RemovedFileSuffixFacet::FacetKey).toString();

            // Build full file path.
            fileName = removedPrefix + fileName + removedSuffix;
            QString projectPath = context.projectController.getProjectPath();

            fileName = combinePaths(projectPath, fileName);

            // Check file.
            QFileInfo file(fileName);

            if (!file.exists())
            {
                Message message;
                message.content = tr("The file %1 referenced by record %2 field %3 does not exist.").arg(fileName, record.id, fieldId);
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
