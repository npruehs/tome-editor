#include "componentwindow.h"
#include "ui_componentwindow.h"

#include <QMessageBox>


ComponentWindow::ComponentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentWindow)
{
    ui->setupUi(this);
}

ComponentWindow::~ComponentWindow()
{
    delete this->ui;
}

QString ComponentWindow::getComponentName() const
{
    return this->ui->lineEdit->text();
}

QString ComponentWindow::getComponentSetName() const
{
    return this->ui->comboBox->currentText();
}

void ComponentWindow::setDisallowedComponentIds(const QStringList disallowedComponentIds)
{
    this->disallowedComponentIds = disallowedComponentIds;
}

void ComponentWindow::setComponentSetName(const QString& componentSetName)
{
    this->ui->comboBox->setCurrentText(componentSetName);
}

void ComponentWindow::setComponentSetNames(const QStringList& componentSetNames)
{
    this->ui->comboBox->clear();
    this->ui->comboBox->addItems(componentSetNames);
}

void ComponentWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

void ComponentWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    // Enable user to input data immediately.
    this->ui->lineEdit->setFocus();
}

bool ComponentWindow::validate()
{
    // Name must not be empty.
    if (this->getComponentName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the component."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Component ids must be unique.
    if (this->disallowedComponentIds.contains(this->getComponentName()))
    {
        QMessageBox::information(
                    this,
                    tr("Duplicate component id"),
                    tr("Please specify another id for the component."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
