#include "customtypeswindow.h"
#include "ui_customtypeswindow.h"

#include "builtintype.h"
#include "../Util/vectorutils.h"

using namespace Tome;


bool lessThanCustomTypes(const QSharedPointer<CustomType>& e1, const QSharedPointer<CustomType>& e2)
{
    return e1->name.toLower() < e2->name.toLower();
}


CustomTypesWindow::CustomTypesWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomTypesWindow),
    enumerationWindow(0),
    listWindow(0),
    project(project)
{
    ui->setupUi(this);

    // Setup view.
    this->ui->tableWidget->setRowCount(this->project->types.size());
    this->ui->tableWidget->setColumnCount(3);

    QStringList headers;
    headers << tr("Id");
    headers << tr("Type");
    headers << tr("Details");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < this->project->types.size(); ++i)
    {
        this->updateRow(i);
    }
}

CustomTypesWindow::~CustomTypesWindow()
{
    delete ui;
}

void CustomTypesWindow::on_actionNew_Custom_Type_triggered()
{
    // Show window.
    if (!this->enumerationWindow)
    {
        this->enumerationWindow = new EnumerationWindow(this);
    }

    int result = this->enumerationWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update model.
        QSharedPointer<CustomType> newType = QSharedPointer<CustomType>::create();
        newType->name = this->enumerationWindow->getEnumerationName();
        newType->baseType = BuiltInType::String;
        newType->setEnumeration(this->enumerationWindow->getEnumerationMembers());

        int index = findInsertionIndex(this->project->types, newType, lessThanCustomTypes);
        this->project->types.insert(index, newType);

        // Update view.
        this->ui->tableWidget->insertRow(index);
        this->updateRow(index);
    }
}

void CustomTypesWindow::on_actionNew_List_triggered()
{
    // Show window.
    if (!this->listWindow)
    {
        this->listWindow = new ListWindow(this);
    }

    // Update type selection.
    this->listWindow->setProject(this->project);

    int result = this->listWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update model.
        QSharedPointer<CustomType> newType = QSharedPointer<CustomType>::create();
        newType->name = this->listWindow->getListName();
        newType->baseType = BuiltInType::None;
        newType->setItemType(this->listWindow->getListItemType());

        int index = findInsertionIndex(this->project->types, newType, lessThanCustomTypes);
        this->project->types.insert(index, newType);

        // Update view.
        this->ui->tableWidget->insertRow(index);
        this->updateRow(index);
    }
}

void CustomTypesWindow::on_actionEdit_Custom_Type_triggered()
{
    // Get selected type.
    int index = getSelectedTypeIndex();

    if (index < 0)
    {
        return;
    }

    QSharedPointer<CustomType> type = this->project->types[index];

    // Check type.
    if (type->isEnumeration())
    {
        this->editEnumeration(index, type);
    }
    else if (type->isList())
    {
        this->editList(index, type);
    }
}

void CustomTypesWindow::on_actionDelete_Custom_Type_triggered()
{
    // Get selected type.
    int index = getSelectedTypeIndex();

    if (index < 0)
    {
        return;
    }

    // Update model.
    this->project->types.removeAt(index);

    // Update view.
    this->ui->tableWidget->removeRow(index);
}

void CustomTypesWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Custom_Type_triggered();
}

int CustomTypesWindow::getSelectedTypeIndex() const
{
    QModelIndexList selectedIndexes = this->ui->tableWidget->selectionModel()->selectedRows();
    return selectedIndexes.count() > 0 ? selectedIndexes.first().row() : -1;
}

void CustomTypesWindow::editEnumeration(int index, QSharedPointer<Tome::CustomType> type)
{
    // Show window.
    if (!this->enumerationWindow)
    {
        this->enumerationWindow = new EnumerationWindow(this);
    }

    // Update view.
    this->enumerationWindow->setEnumerationName(type->name);
    this->enumerationWindow->setEnumerationMembers(type->getEnumeration());

    int result = this->enumerationWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->updateEnumeration(
                    index,
                    this->enumerationWindow->getEnumerationName(),
                    this->enumerationWindow->getEnumerationMembers());
    }
}

void CustomTypesWindow::editList(int index, QSharedPointer<Tome::CustomType> type)
{
    // Show window.
    if (!this->listWindow)
    {
        this->listWindow = new ListWindow(this);
    }

    // Update type selection.
    this->listWindow->setProject(this->project);

    // Update view.
    this->listWindow->setListName(type->name);
    this->listWindow->setListItemType(type->getItemType());

    int result = this->listWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->updateList(
                    index,
                    this->listWindow->getListName(),
                    this->listWindow->getListItemType());
    }
}

void CustomTypesWindow::updateEnumeration(const int index, const QString& name, const QStringList& enumeration)
{
    QSharedPointer<CustomType>& type = this->project->types[index];

    bool needsSorting = type->name != name;

    // Update model.
    type->name = name;
    type->setEnumeration(enumeration);

    // Update view.
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);
        this->ui->tableWidget->sortItems(0);
    }
}

void CustomTypesWindow::updateList(int index, const QString& name, const QString& itemType)
{
    QSharedPointer<CustomType>& type = this->project->types[index];

    bool needsSorting = type->name != name;

    // Update model.
    type->name = name;
    type->setItemType(itemType);

    // Update view.
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);
        this->ui->tableWidget->sortItems(0);
    }
}

void CustomTypesWindow::updateRow(const int index)
{
    QVector<QSharedPointer<CustomType> >& types = this->project->types;
    QSharedPointer<CustomType> type = types[index];

    this->ui->tableWidget->setItem(index, 0, new QTableWidgetItem(type->name));

    if (type->isEnumeration())
    {
        this->ui->tableWidget->setItem(index, 1, new QTableWidgetItem("Enumeration"));
        this->ui->tableWidget->setItem(index, 2, new QTableWidgetItem(type->getEnumeration().join(", ")));
    }
    else if (type->isList())
    {
        this->ui->tableWidget->setItem(index, 1, new QTableWidgetItem("List"));
        this->ui->tableWidget->setItem(index, 2, new QTableWidgetItem(type->getItemType()));
    }
}
