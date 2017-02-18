#include "recordfieldstablewidget.h"

#include <QHeaderView>
#include <QLabel>

#include "../Controller/recordscontroller.h"
#include "../Model/recordfieldvaluemap.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../Types/Model/customtype.h"
#include "../../../Util/listutils.h"
#include "../../../Util/stringutils.h"

using namespace Tome;


RecordFieldsTableWidget::RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
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

void RecordFieldsTableWidget::setRecord(const QString recordId)
{
    this->recordId = recordId;

    // Get field values.
    const RecordFieldValueMap fieldValues =
            this->recordsController.getRecordFieldValues(recordId);

    // Sort by display name and component.
    FieldDefinitionList fields;

    for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
         it != fieldValues.end();
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
    QString valueString = this->typesController.valueToString(value, field.fieldType);

    // Show field name.
    this->item(i, 0)->setData(Qt::DisplayRole, keyString);

    // Show hyperlink for reference fields, and normal text for other fields.
    if (this->typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::Reference))
    {
        QString href = "<a href='" + valueString + "'>" + valueString + "</a>";
        QModelIndex index = this->model()->index(i, 1);
        // Check for an existing index widget
        QWidget *indexWidget = this->indexWidget(index);
        if (nullptr != indexWidget)
        {
            QLabel* valueLabel = static_cast<QLabel*>(indexWidget);
            valueLabel->setText(href);
        }
        // Create a new index widget
        else
        {
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
    }
    else
    {
        // Remove any index widget
        QModelIndex index = this->model()->index(i, 1);
        QWidget *indexWidget = this->indexWidget(index);
        if (nullptr != indexWidget)
        {
            this->setIndexWidget(index, nullptr);
            delete indexWidget;
        }

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

void RecordFieldsTableWidget::onRecordLinkActivated(const QString& recordId)
{
    emit this->recordLinkActivated(recordId);
}
