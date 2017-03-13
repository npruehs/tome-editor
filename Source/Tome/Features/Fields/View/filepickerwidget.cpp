#include "filepickerwidget.h"

#include <QDir>
#include <QFileDialog>

#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Controller/removedfileprefixfacet.h"
#include "../../Facets/Controller/removedfilesuffixfacet.h"
#include "../../Projects/Controller/projectcontroller.h"
#include "../../../Util/memoryutils.h"
#include "../../../Util/pathutils.h"


using namespace Tome;


FilePickerWidget::FilePickerWidget(FacetsController& facetsController, ProjectController& projectController, QWidget* parent)
    : QWidget(parent),
      facetsController(facetsController),
      projectController(projectController)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->lineEdit = new QLineEdit();
    this->layout->addWidget(this->lineEdit);

    this->button = new QPushButton();
    this->button->setText(tr("Browse..."));
    this->layout->addWidget(this->button);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);

    // Connect signal.
    connect(
                this->button,
                SIGNAL(clicked(bool)),
                SLOT(onBrowseButtonClicked(bool))
                );
}

FilePickerWidget::~FilePickerWidget()
{
    deleteLayout(this->layout);
}

QVariant FilePickerWidget::getFileName() const
{
    return this->lineEdit->text();
}

void FilePickerWidget::setFileName(const QVariant& v)
{
    this->lineEdit->setText(v.toString());
}

void FilePickerWidget::setFieldType(const QString& fieldType)
{
    this->fieldType = fieldType;
}

void FilePickerWidget::onBrowseButtonClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get removed prefix and suffix.
    QString removedPrefix;
    QString removedSuffix;

    if (!this->fieldType.isEmpty())
    {
        removedPrefix = this->facetsController.getFacetValue(this->fieldType, RemovedFilePrefixFacet::FacetKey).toString();
        removedSuffix = this->facetsController.getFacetValue(this->fieldType, RemovedFileSuffixFacet::FacetKey).toString();
    }

    // Build current file path.
    QString fileName = removedPrefix + this->getFileName().toString() + removedSuffix;
    QString projectPath = this->projectController.getProjectPath();

    fileName = combinePaths(projectPath, fileName);

    // Open file browser dialog.
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Select File"),
                                            fileName);

    if (fileName.isEmpty())
    {
        return;
    }

    // Get relative path.
    QDir projectDir(projectPath);
    fileName = projectDir.relativeFilePath(fileName);

    // Strip prefix and suffix.
    if (fileName.startsWith(removedPrefix))
    {
        fileName.remove(0, removedPrefix.length());
    }

    if (fileName.endsWith(removedSuffix))
    {
        fileName.remove(fileName.length() - removedSuffix.length(), removedSuffix.length());
    }

    // Set selected file.
    this->setFileName(fileName);
}
