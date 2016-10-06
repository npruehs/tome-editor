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

using namespace Tome;


RecordFieldsTableWidget::RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
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

void RecordFieldsTableWidget::setRecord(int i, const QString recordId)
{
    // Get record field values.
    const RecordFieldValueMap fieldValues =
            this->recordsController.getRecordFieldValues(recordId);

    // Get selected record field key and value.
    QString key = fieldValues.keys()[i];
    QVariant value = fieldValues[key];

    // Get selected record field type.
    const FieldDefinition& field =
            this->fieldDefinitionsController.getFieldDefinition(key);

    QString keyString = field.displayName;
    QString valueString = this->typesController.valueToString(value, field.fieldType);

    // Show field, value and description.
    this->setItem(i, 0, new QTableWidgetItem(keyString));
    this->setItem(i, 1, new QTableWidgetItem());

    if (this->columnCount() > 2)
    {
        // Show field description as column.
        this->setItem(i, 2, new QTableWidgetItem(field.description));
    }
    else
    {
        // Show field description as tooltip.
        this->item(i, 0)->setData(Qt::ToolTipRole, field.description);
        this->item(i, 1)->setData(Qt::ToolTipRole, field.description);
    }

    // Show hyperlink for reference fields, and normal text for other fields.
    if (field.fieldType == BuiltInType::Reference)
    {
        QLabel* valueLabel = new QLabel("<a href='" + valueString + "'>" + valueString + "</a>");

        connect(
                    valueLabel,
                    SIGNAL(linkActivated(const QString&)),
                    SLOT(onRecordLinkActivated(const QString&))
                    );

        // Add margin for increased readability.
        valueLabel->setMargin(5);

        QModelIndex index = this->model()->index(i, 1);
        this->setIndexWidget(index, valueLabel);
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

    // Resize columns.
    this->horizontalHeader()->setStretchLastSection(true);
}

void RecordFieldsTableWidget::onRecordLinkActivated(const QString& recordId)
{
    emit this->recordLinkActivated(recordId);
}
