#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include "../Types/builtintype.h"
#include "../Util/vectorutils.h"

using namespace Tome;


bool lessThanFieldDefinitions(const QSharedPointer<FieldDefinition>& e1, const QSharedPointer<FieldDefinition>& e2)
{
    return e1->displayName.toLower() < e2->displayName.toLower();
}

FieldDefinitionsWindow::FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionWindow(0),
    project(project)
{
    ui->setupUi(this);

    // Setup view.
    QVector<QSharedPointer<FieldDefinition> >& fieldDefinitions = project->fieldDefinitionSets[0]->fieldDefinitions;

    this->ui->tableWidget->setRowCount(fieldDefinitions.size());
    this->ui->tableWidget->setColumnCount(6);

    QStringList headers;
    headers << tr("Id");
    headers << tr("Display Name");
    headers << tr("Type");
    headers << tr("Default Value");
    headers << tr("Component");
    headers << tr("Description");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        this->updateRow(i);
    }

    // Listen for selection changes.
    connect(
                this->ui->tableWidget->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(tableWidgetSelectionChanged(const QItemSelection &, const QItemSelection &))
                );

    this->updateMenus();
}

FieldDefinitionsWindow::~FieldDefinitionsWindow()
{
    delete ui;
}

void FieldDefinitionsWindow::on_actionNew_Field_triggered()
{
    // Setup window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(this);
    }

    // Update project reference.
    this->fieldDefinitionWindow->setProject(this->project);

    // Show window.
    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update model.
        QSharedPointer<FieldDefinition> fieldDefinition = QSharedPointer<Tome::FieldDefinition>::create();
        fieldDefinition->id = this->fieldDefinitionWindow->getFieldId();
        fieldDefinition->displayName = this->fieldDefinitionWindow->getFieldDisplayName();
        fieldDefinition->fieldType = this->fieldDefinitionWindow->getFieldType();
        fieldDefinition->defaultValue = this->fieldDefinitionWindow->getDefaultValue();
        fieldDefinition->component = this->fieldDefinitionWindow->getFieldComponent();
        fieldDefinition->description = this->fieldDefinitionWindow->getFieldDescription();

        QVector<QSharedPointer<FieldDefinition> >& fieldDefinitions = this->project->fieldDefinitionSets[0]->fieldDefinitions;
        int index = findInsertionIndex(fieldDefinitions, fieldDefinition, lessThanFieldDefinitions);
        fieldDefinitions.insert(index, fieldDefinition);

        // Update view.
        this->ui->tableWidget->insertRow(index);
        this->updateFieldDefinition(
                    index,
                    fieldDefinition->id,
                    fieldDefinition->displayName,
                    fieldDefinition->fieldType,
                    fieldDefinition->defaultValue,
                    fieldDefinition->description,
                    fieldDefinition->component);
    }
}

void FieldDefinitionsWindow::on_actionEdit_Field_triggered()
{
    int index = getSelectedFieldIndex();

    if (index < 0)
    {
        return;
    }

    // Get selected field definition.
    QSharedPointer<FieldDefinition> fieldDefinition =
            this->project->fieldDefinitionSets[0]->fieldDefinitions[index];

    // Show window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(this);
    }

    // Update project reference.
    this->fieldDefinitionWindow->setProject(this->project);

    // Update view.
    this->fieldDefinitionWindow->setFieldId(fieldDefinition->id);
    this->fieldDefinitionWindow->setFieldDisplayName(fieldDefinition->displayName);
    this->fieldDefinitionWindow->setFieldType(fieldDefinition->fieldType);
    this->fieldDefinitionWindow->setDefaultValue(fieldDefinition->defaultValue);
    this->fieldDefinitionWindow->setFieldDescription(fieldDefinition->description);
    this->fieldDefinitionWindow->setFieldComponent(fieldDefinition->component);

    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update field.
        this->updateFieldDefinition
                (index,
                 this->fieldDefinitionWindow->getFieldId(),
                 this->fieldDefinitionWindow->getFieldDisplayName(),
                 this->fieldDefinitionWindow->getFieldType(),
                 this->fieldDefinitionWindow->getDefaultValue(),
                 this->fieldDefinitionWindow->getFieldDescription(),
                 this->fieldDefinitionWindow->getFieldComponent());
    }
}

void FieldDefinitionsWindow::on_actionDelete_Field_triggered()
{
    int index = getSelectedFieldIndex();

    if (index < 0)
    {
        return;
    }

    // Update model.
    this->project->fieldDefinitionSets[0]->fieldDefinitions.removeAt(index);

    // Update view.
    this->ui->tableWidget->removeRow(index);
}

void FieldDefinitionsWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Field_triggered();
}

void FieldDefinitionsWindow::tableWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    this->updateMenus();
}

int FieldDefinitionsWindow::getSelectedFieldIndex() const
{
    return this->ui->tableWidget->currentRow();
}

void FieldDefinitionsWindow::updateMenus()
{
    bool hasSelection = getSelectedFieldIndex() >= 0;

    this->ui->actionEdit_Field->setEnabled(hasSelection);
    this->ui->actionDelete_Field->setEnabled(hasSelection);
}

void FieldDefinitionsWindow::updateFieldDefinition(const int index, const QString& id, const QString& displayName, const QString& fieldType, const QString& defaultValue, const QString& description, const QString& component)
{
    QVector<QSharedPointer<FieldDefinition> >& fieldDefinitions = this->project->fieldDefinitionSets[0]->fieldDefinitions;
    QSharedPointer<FieldDefinition> fieldDefinition = fieldDefinitions[index];

    bool needsSorting = fieldDefinition->displayName != displayName;

    // Update model.
    fieldDefinition->id = id;
    fieldDefinition->displayName = displayName;
    fieldDefinition->fieldType = fieldType;
    fieldDefinition->defaultValue = defaultValue;
    fieldDefinition->description = description;
    fieldDefinition->component = component;

    // Update view.
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        std::sort(fieldDefinitions.begin(), fieldDefinitions.end(), lessThanFieldDefinitions);
        this->ui->tableWidget->sortItems(1);
    }
}

void FieldDefinitionsWindow::updateRow(const int i)
{
    QVector<QSharedPointer<FieldDefinition> >& fieldDefinitions = project->fieldDefinitionSets[0]->fieldDefinitions;
    QSharedPointer<FieldDefinition> fieldDefinition = fieldDefinitions[i];

    this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(fieldDefinition->id));
    this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(fieldDefinition->displayName));
    this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(fieldDefinition->fieldType));
    this->ui->tableWidget->setItem(i, 3, new QTableWidgetItem(fieldDefinition->defaultValue));
    this->ui->tableWidget->setItem(i, 4, new QTableWidgetItem(fieldDefinition->component));
    this->ui->tableWidget->setItem(i, 5, new QTableWidgetItem(fieldDefinition->description));

    if (fieldDefinition->fieldType == BuiltInType::Color)
    {
        // Show color preview.
        QColor color;
        color.setNamedColor(fieldDefinition->defaultValue);
        this->ui->tableWidget->item(i, 3)->setData(Qt::DecorationRole, color);
    }
}
