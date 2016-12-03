#include "componentswindow.h"
#include "ui_componentswindow.h"

#include "componentwindow.h"
#include "../Controller/componentscontroller.h"
#include "../Controller/Commands/addcomponentcommand.h"
#include "../Model/component.h"
#include "../Model/componentlist.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Undo/Controller/undocontroller.h"

using namespace Tome;


ComponentsWindow::ComponentsWindow(ComponentsController& componentsController, FieldDefinitionsController& fieldDefinitionsController, UndoController& undoController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComponentsWindow),
    componentsController(componentsController),
    fieldDefinitionsController(fieldDefinitionsController),
    undoController(undoController),
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

    // Connect signals.
    connect(
                &this->componentsController,
                SIGNAL(componentAdded(const Tome::Component&)),
                SLOT(onComponentAdded(const Tome::Component&)));

    connect(
                &this->componentsController,
                SIGNAL(componentRemoved(const Tome::Component&)),
                SLOT(onComponentRemoved(const Tome::Component&)));
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

    // Set component sets.
    const QStringList componentSetNames = this->componentsController.getComponentSetNames();
    const QStringList componentIds = this->componentsController.getComponents();
    this->componentWindow->setComponentSetNames(componentSetNames);
    this->componentWindow->setComponentSetName(componentSetNames.first());
    this->componentWindow->setDisallowedComponentIds(componentIds);

    int result = this->componentWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString& componentName = this->componentWindow->getComponentName();
        const QString& componentSetName = this->componentWindow->getComponentSetName();

        // Update model.
        AddComponentCommand* command =
                new AddComponentCommand(this->componentsController, componentName, componentSetName);
        this->undoController.doCommand(command);
    }
}

void ComponentsWindow::on_actionDelete_Component_triggered()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget->selectedItems();

    if (selectedItems.isEmpty())
    {
        return;
    }

    Component component = selectedItems.first()->text();

    // Update model.
    this->componentsController.removeComponent(component);
    this->fieldDefinitionsController.removeFieldComponent(component);
}

void ComponentsWindow::onComponentAdded(const Component& component)
{
    // Update view.
    int index = this->componentsController.indexOf(component);
    this->ui->listWidget->insertItem(index, component);
}

void ComponentsWindow::onComponentRemoved(const Component& component)
{
    // Update view.
    int index = this->componentsController.indexOf(component);
    this->ui->listWidget->takeItem(index);
}
