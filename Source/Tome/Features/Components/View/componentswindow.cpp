#include "componentswindow.h"
#include "ui_componentswindow.h"

#include "componentwindow.h"
#include "../Controller/componentscontroller.h"
#include "../Model/component.h"
#include "../Model/componentlist.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"

using namespace Tome;


ComponentsWindow::ComponentsWindow(ComponentsController& componentsController, FieldDefinitionsController& fieldDefinitionsController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComponentsWindow),
    componentsController(componentsController),
    fieldDefinitionsController(fieldDefinitionsController),
    componentWindow(0)
{
    ui->setupUi(this);

    // Setup view.
    const ComponentList& components = this->componentsController.getComponents();

    for (int i = 0; i < components.size(); ++i)
    {
        const Component& component = components.at(i);
        this->ui->listWidget->insertItem(i, component);
    }
}

ComponentsWindow::~ComponentsWindow()
{
    delete this->ui;

    delete this->componentWindow;
}

void ComponentsWindow::on_actionNew_Component_triggered()
{
    // Show window.
    if (!this->componentWindow)
    {
        this->componentWindow = new ComponentWindow(this);
    }

    int result = this->componentWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString& componentName = this->componentWindow->getComponentName();

        // Update model.
        const Component& component = this->componentsController.addComponent(componentName);

        // Update view.
        int index = this->componentsController.indexOf(component);
        this->ui->listWidget->insertItem(index, componentName);
    }
}

void ComponentsWindow::on_actionDelete_Component_triggered()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget->selectedItems();

    if (selectedItems.isEmpty())
    {
        return;
    }

    Component component = selectedItems[0]->text();
    int index = this->componentsController.indexOf(component);

    // Update model.
    this->componentsController.removeComponent(component);
    this->fieldDefinitionsController.removeFieldComponent(component);

    // Update view.
    this->ui->listWidget->takeItem(index);
}
