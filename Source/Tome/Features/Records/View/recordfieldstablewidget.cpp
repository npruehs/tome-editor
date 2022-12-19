#include "recordfieldstablewidget.h"

#include <QHeaderView>
#include <QImageReader>
#include <QLabel>
#include <QPixmap>

#include "labeledpixmapwidget.h"
#include "../Controller/recordscontroller.h"
#include "../Model/recordfieldvaluemap.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Controller/removedfileprefixfacet.h"
#include "../../Facets/Controller/removedfilesuffixfacet.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Projects/Controller/projectcontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../Types/Model/customtype.h"
#include "../../../Util/listutils.h"
#include "../../../Util/pathutils.h"
#include "../../../Util/stringutils.h"


using namespace Tome;


RecordFieldsTableWidget::RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController,
                                                 FacetsController& facetsController,
                                                 ProjectController& projectController,
                                                 RecordsController& recordsController,
                                                 TypesController& typesController)
    : facetsController(facetsController),
      fieldDefinitionsController(fieldDefinitionsController),
      projectController(projectController),
      recordsController(recordsController),
      typesController(typesController),
      recordId(QString()),
      showComponentNames(false)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setVisible(false);
}

void RecordFieldsTableWidget::setDescriptionColumnEnabled(bool enabled)
{
    // Set headers.
    QStringList headers;
    headers << tr("Field");
    headers << tr("Value");

    if (enabled)
    {
        headers << tr("Description");
    }

    this->setColumnCount(headers.count());
    this->setHorizontalHeaderLabels(headers);
}

void RecordFieldsTableWidget::setRecord(const QVariant recordId)
{
    this->recordId = recordId;

    // Get field values.
    const RecordFieldValueMap fieldValues =
            this->recordsController.getRecordFieldValues(recordId);

    // Sort by display name and component.
    FieldDefinitionList fields;

    for (RecordFieldValueMap::const_iterator it = fieldValues.cbegin();
         it != fieldValues.cend();
         ++it)
    {
        const QString fieldId = it.key();
        const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(fieldId);

        fields << field;
    }

    if (this->showComponentNames)
    {
        std::sort(fields.begin(), fields.end(), fieldDefinitionLessThanComponentAndDisplayName);
    }

    // Populate table.
    this->setRowCount(fields.size());

    for (int i = 0; i < this->rowCount(); ++i)
    {
        for (int j = 0; j < this->columnCount(); ++j)
        {
            this->setItem(i, j, new QTableWidgetItem());

            // Store field id in UserRole, so we always know which row is representing which field.
            this->item(i, j)->setData(Qt::UserRole, fields[i].id);
        }
    }

    // Update entries.
    for (int i = 0; i < this->rowCount(); ++i)
    {
        this->updateFieldValue(i);
    }

    // Resize first column.
    this->resizeColumnToContents(0);
}

void RecordFieldsTableWidget::setShowComponentNames(bool showComponentNames)
{
    this->showComponentNames = showComponentNames;
}

void RecordFieldsTableWidget::updateFieldValue(int i)
{
    // Get record field values.
    const RecordFieldValueMap fieldValues =
            this->recordsController.getRecordFieldValues(recordId);

    // Get selected record field key and value.
    QString key = this->item(i, 0)->data(Qt::UserRole).toString();
    QVariant value = fieldValues[key];

    // Get selected record field type.
    const FieldDefinition& field =
            this->fieldDefinitionsController.getFieldDefinition(key);

    // Compose key string.
    QString keyString = this->getFieldKeyString(field);

    // Compose value string.
    QString valueString;

    if (this->typesController.isCustomType(field.fieldType))
    {
        const CustomType& customType = this->typesController.getCustomType(field.fieldType);

        if (customType.isList())
        {
            if (this->typesController.isTypeOrDerivedFromType(customType.getItemType(), BuiltInType::Reference))
            {
                // Replace by list of record display names.
                QVariantList recordIds = value.toList();
                QVariantList recordDisplayNames;

                for (const QVariant& recordId : recordIds)
                {
                    if (this->recordsController.hasRecord(recordId))
                    {
                        const Record& record = this->recordsController.getRecord(recordId);
                        recordDisplayNames << record.displayName;
                    }
                    else
                    {
                        recordDisplayNames << recordId.toString();
                    }
                }

                valueString = toString(recordDisplayNames);
            }
        }
        else if (customType.isMap())
        {
            bool hasReferenceKeys = this->typesController.isTypeOrDerivedFromType(customType.getKeyType(), BuiltInType::Reference);
            bool hasReferenceValues = this->typesController.isTypeOrDerivedFromType(customType.getValueType(), BuiltInType::Reference);

            if (hasReferenceKeys || hasReferenceValues)
            {
                // Replace by map of record display names.
                QVariantMap valueMap = value.toMap();
                QVariantMap valueMapWithDisplayNames;

                for (QVariantMap::const_iterator it = valueMap.cbegin();
                     it != valueMap.cend();
                     ++it)
                {
                    // Make key string.
                    QString keyString;

                    if (hasReferenceKeys)
                    {
                        QVariant recordId = QVariant(it.key());

                        if (this->recordsController.hasRecord(recordId))
                        {
                            const Record& record = this->recordsController.getRecord(recordId);
                            keyString = record.displayName;
                        }
                        else
                        {
                            keyString = recordId.toString();
                        }
                    }
                    else
                    {
                        keyString = it.key();
                    }

                    // Make value string.
                    QString valueString;

                    if (hasReferenceValues)
                    {
                        QVariant recordId = it.value();

                        if (this->recordsController.hasRecord(recordId))
                        {
                            const Record& record = this->recordsController.getRecord(recordId);
                            valueString = record.displayName;
                        }
                        else
                        {
                            valueString = recordId.toString();
                        }
                    }
                    else
                    {
                        valueString = it.value().toString();
                    }

                    // Insert into display map.
                    valueMapWithDisplayNames.insert(keyString, valueString);
                }

                valueString = toString(valueMapWithDisplayNames);
            }
        }
    }

    if (valueString.isEmpty())
    {
        valueString = this->typesController.valueToString(value, field.fieldType);
    }

    // Show field name.
    this->item(i, 0)->setData(Qt::DisplayRole, keyString);

    // Remove any existing index widget.
    QModelIndex index = this->model()->index(i, 1);
    QWidget *indexWidget = this->indexWidget(index);
    if (nullptr != indexWidget)
    {
        this->setIndexWidget(index, nullptr);
        delete indexWidget;
    }

    // Show hyperlink for reference fields, and normal text for other fields.
    if (this->typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::Reference))
    {
        QString href;
        QString valueString = value.toString();

        if (!valueString.isEmpty())
        {
            if (this->recordsController.hasRecord(value))
            {
                const Record& record = this->recordsController.getRecord(value);
                href = QString("<a href='%1'>%2</a>").arg(record.id.toString(), record.displayName);
            }else{
                href = QString("<i><font color='red'>INVALID REFERENCE</font></i>");
            }
        }

        QModelIndex index = this->model()->index(i, 1);

        // Create new index widget.
        QLabel* valueLabel = new QLabel(href);

        connect(
                    valueLabel,
                    SIGNAL(linkActivated(const QString&)),
                    SLOT(onRecordLinkActivated(const QString&))
                    );

        // Add margin for increased readability.
        valueLabel->setMargin(5);
        this->setIndexWidget(index, valueLabel);
    }
    else if (this->typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::File))
    {
        // Build full file path.
        QString removedPrefix;
        QString removedSuffix;

        removedPrefix = this->facetsController.getFacetValue(field.fieldType, RemovedFilePrefixFacet::FacetKey).toString();
        removedSuffix = this->facetsController.getFacetValue(field.fieldType, RemovedFileSuffixFacet::FacetKey).toString();

        QString fileName = removedPrefix + valueString + removedSuffix;
        QString projectPath = this->projectController.getProjectPath();

        fileName = combinePaths(projectPath, fileName);

        QString fileNameLower = fileName.toLower();

        // Get preview.
        QPixmap preview;

        QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
        for (QByteArray& format : supportedImageFormats)
        {
            if (fileNameLower.endsWith(format))
            {
                preview.load(fileName);

                if (!preview.isNull())
                {
                    break;
                }
            }
        }

        // Show file path.
        QString href = "<a href='" + fileName + "'>" + valueString + "</a>";
        QWidget* widget;

        if (!preview.isNull())
        {
            // Create new labeled pixmap.
            LabeledPixmapWidget* labeledPixmap = new LabeledPixmapWidget(this);
            labeledPixmap->setText(href);
            labeledPixmap->setPixmap(preview);

            connect(
                        &labeledPixmap->getTextLabel(),
                        SIGNAL(linkActivated(const QString&)),
                        SLOT(onFileLinkActivated(const QString&))
                        );

            // Add margin for increased readability.
            labeledPixmap->getTextLabel().setMargin(5);

            widget = labeledPixmap;
        }
        else
        {
            QLabel* valueLabel = new QLabel(href);

            connect(
                        valueLabel,
                        SIGNAL(linkActivated(const QString&)),
                        SLOT(onFileLinkActivated(const QString&))
                        );

            // Add margin for increased readability.
            valueLabel->setMargin(5);

            widget = valueLabel;
        }

        QModelIndex index = this->model()->index(i, 1);
        this->setIndexWidget(index, widget);
    }
    else
    {
        // Show normal text.
        this->item(i, 1)->setData(Qt::DisplayRole, valueString);

        // Show color preview.
        if (field.fieldType == BuiltInType::Color)
        {
            QColor color = value.value<QColor>();
            this->item(i, 1)->setData(Qt::DecorationRole, color);
        }
    }

    // Show field description.
    if (this->columnCount() > 2)
    {
        // Show field description as column.
        this->item(i, 2)->setData(Qt::DisplayRole, field.description);
    }
    else
    {
        // Show field description as tooltip.
        this->item(i, 0)->setData(Qt::ToolTipRole, field.description);
        this->item(i, 1)->setData(Qt::ToolTipRole, field.description);
    }

    // Resize columns.
    this->horizontalHeader()->setStretchLastSection(true);
}

const QString RecordFieldsTableWidget::getFieldKeyString(const FieldDefinition& field)
{
    if (!this->showComponentNames || field.component.isEmpty())
    {
        return field.displayName;
    }

    QString shortComponentName = field.component;
    shortComponentName.replace("component", "");
    shortComponentName.replace("Component", "");
    shortComponentName = splitAtCapitalLetters(shortComponentName);
    return QString("%1 - %2").arg(shortComponentName, field.displayName);
}

void RecordFieldsTableWidget::onFileLinkActivated(const QString& filePath)
{
    emit this->fileLinkActivated(filePath);
}

void RecordFieldsTableWidget::onRecordLinkActivated(const QString& recordId)
{
    emit this->recordLinkActivated(recordId);
}
