#include "componentswindow.h"
#include "ui_componentswindow.h"

#include "../Model/component.h"
#include "../Model/componentlist.h"

using namespace Tome;


ComponentsWindow::ComponentsWindow(ComponentsController& controller, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComponentsWindow),
    controller(controller),
    componentWindow(0)
{
    ui->setupUi(this);

    // Setup view.
    const ComponentList& components = this->controller.getComponents();

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
        const Component& component = this->controller.addComponent(componentName);

        // Update view.
        int index = this->controller.indexOf(component);
        this->ui->listWidget->insertItem(index, componentName);
    }
}

void ComponentsWindow::on_actionDelete_Component_triggered()
{
    int index = this->ui->listWidget->currentRow();

    // Update model.
    this->controller.removeComponentAt(index);

    // Update view.
    this->ui->listWidget->takeItem(index);
}
