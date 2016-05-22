#include "recordfieldstablewidget.h"

#include <QHeaderView>

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

    // Set headers.
    this->setColumnCount(2);

    QStringList headers;
    headers << tr("Field");
    headers << tr("Value");
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

    // Show field and value.
    this->setItem(i, 0, new QTableWidgetItem(keyString));
    this->setItem(i, 1, new QTableWidgetItem(valueString));

    // Show field description as tooltip.
    this->item(i, 0)->setData(Qt::ToolTipRole, field.description);
    this->item(i, 1)->setData(Qt::ToolTipRole, field.description);

    // Show color preview.
    if (field.fieldType == BuiltInType::Color)
    {
        QColor color = value.value<QColor>();
        this->item(i, 1)->setData(Qt::DecorationRole, color);
    }

    // Resize columns.
    this->resizeColumnsToContents();
    this->horizontalHeader()->setStretchLastSection(true);
}
