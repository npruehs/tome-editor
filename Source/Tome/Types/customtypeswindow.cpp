#include "customtypeswindow.h"
#include "ui_customtypeswindow.h"

#include "builtintype.h"
#include "../Util/vectorutils.h"

using namespace Tome;


CustomTypesWindow::CustomTypesWindow(TypesController& typesController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomTypesWindow),
    typesController(typesController),
    enumerationWindow(0),
    listWindow(0)
{
    ui->setupUi(this);

    // Setup view.
    int typeCount = this->typesController.getCustomTypes().size();

    this->ui->tableWidget->setRowCount(typeCount);
    this->ui->tableWidget->setColumnCount(3);

    QStringList headers;
    headers << tr("Id");
    headers << tr("Type");
    headers << tr("Details");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < typeCount; ++i)
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
        CustomType newType =
                this->typesController.addEnumeration(
                    this->enumerationWindow->getEnumerationName(),
                    this->enumerationWindow->getEnumerationMembers());

        // Update view.
        int index = this->typesController.indexOf(newType);
        this->ui->tableWidget->insertRow(index);
        this->updateRow(index);
    }
}

void CustomTypesWindow::on_actionNew_List_triggered()
{
    // Show window.
    if (!this->listWindow)
    {
        this->listWindow = new ListWindow(this->typesController, this);
    }

    int result = this->listWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update model.
        CustomType newType =
                this->typesController.addList(
                    this->listWindow->getListName(),
                    this->listWindow->getListItemType());

        // Update view.
        int index = this->typesController.indexOf(newType);

        this->ui->tableWidget->insertRow(index);
        this->updateRow(index);
    }
}

void CustomTypesWindow::on_actionEdit_Custom_Type_triggered()
{
    // Get selected type.
    QString typeName = this->getSelectedTypeName();

    if (typeName.isEmpty())
    {
        return;
    }

    const CustomType& type = this->typesController.getCustomType(typeName);

    // Check type.
    if (type.isEnumeration())
    {
        this->editEnumeration(typeName, type);
    }
    else if (type.isList())
    {
        this->editList(typeName, type);
    }
}

void CustomTypesWindow::on_actionDelete_Custom_Type_triggered()
{
    // Get selected type.
    int index = this->getSelectedTypeIndex();

    if (index < 0)
    {
        return;
    }

    // Update model.
    this->typesController.removeCustomTypeAt(index);

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

QString CustomTypesWindow::getSelectedTypeName() const
{
    int selectedIndex = this->getSelectedTypeIndex();

    if (selectedIndex < 0)
    {
        return QString();
    }

    return this->ui->tableWidget->item(selectedIndex, 0)->data(Qt::DisplayRole).toString();
}

void CustomTypesWindow::editEnumeration(QString typeName, const CustomType& type)
{
    // Show window.
    if (!this->enumerationWindow)
    {
        this->enumerationWindow = new EnumerationWindow(this);
    }

    // Update view.
    this->enumerationWindow->setEnumerationName(type.name);
    this->enumerationWindow->setEnumerationMembers(type.getEnumeration());

    int result = this->enumerationWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->updateEnumeration(
                    typeName,
                    this->enumerationWindow->getEnumerationName(),
                    this->enumerationWindow->getEnumerationMembers());
    }
}

void CustomTypesWindow::editList(QString typeName, const CustomType& type)
{
    // Show window.
    if (!this->listWindow)
    {
        this->listWindow = new ListWindow(this->typesController, this);
    }

    // Update view.
    this->listWindow->setListName(type.name);
    this->listWindow->setListItemType(type.getItemType());

    int result = this->listWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->updateList(
                    typeName,
                    this->listWindow->getListName(),
                    this->listWindow->getListItemType());
    }
}

void CustomTypesWindow::updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration)
{
    const CustomType& type = this->typesController.getCustomType(oldName);

    bool needsSorting = type.name != newName;

    // Update model.
    this->typesController.updateEnumeration(oldName, newName, enumeration);

    // Update view.
    int index = this->typesController.indexOf(type);
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        this->ui->tableWidget->sortItems(0);
    }
}

void CustomTypesWindow::updateList(const QString& oldName, const QString& newName, const QString& itemType)
{
    const CustomType& type = this->typesController.getCustomType(oldName);

    bool needsSorting = type.name != newName;

    // Update model.
    this->typesController.updateList(oldName, newName, itemType);

    // Update view.
    int index = this->typesController.indexOf(type);
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        this->ui->tableWidget->sortItems(0);
    }
}

void CustomTypesWindow::updateRow(const int index)
{
    const CustomTypeList& types = this->typesController.getCustomTypes();
    const CustomType& type = types[index];

    this->ui->tableWidget->setItem(index, 0, new QTableWidgetItem(type.name));

    if (type.isEnumeration())
    {
        this->ui->tableWidget->setItem(index, 1, new QTableWidgetItem("Enumeration"));
        this->ui->tableWidget->setItem(index, 2, new QTableWidgetItem(type.getEnumeration().join(", ")));
    }
    else if (type.isList())
    {
        this->ui->tableWidget->setItem(index, 1, new QTableWidgetItem("List"));
        this->ui->tableWidget->setItem(index, 2, new QTableWidgetItem(type.getItemType()));
    }
}
