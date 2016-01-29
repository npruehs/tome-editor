#include "componentswindow.h"
#include "ui_componentswindow.h"

#include "../Util/vectorutils.h"

using namespace Tome;

ComponentsWindow::ComponentsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComponentsWindow),
    componentWindow(0),
    project(project)
{
    ui->setupUi(this);

    // Setup view.
    QVector<QString>& components = this->project->components;

    for (int i = 0; i < components.size(); ++i)
    {
        QString component = components.at(i);
        this->ui->listWidget->insertItem(i, component);
    }
}

ComponentsWindow::~ComponentsWindow()
{
    delete ui;
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
        QString componentName = this->componentWindow->getComponentName();

        // Find insertion index.
        int index = findInsertionIndex(this->project->components, componentName);

        // Update model.
        QVector<QString>& components = this->project->components;
        components.insert(index, componentName);

        // Update view.
        this->ui->listWidget->insertItem(index, componentName);
    }
}

void ComponentsWindow::on_actionDelete_Component_triggered()
{
    int index = this->ui->listWidget->currentRow();

    // Update model.
    this->project->components.removeAt(index);

    // Update view.
    this->ui->listWidget->takeItem(index);
}
