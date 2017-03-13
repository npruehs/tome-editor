#ifndef FILEPICKERWIDGET_H
#define FILEPICKERWIDGET_H

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class FacetsController;
    class ProjectController;

    class FilePickerWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit FilePickerWidget(FacetsController& facetsController, ProjectController& projectController, QWidget* parent = 0);
            ~FilePickerWidget();

            QVariant getFileName() const;
            void setFileName(const QVariant& v);

            void setFieldType(const QString& fieldType);

        private slots:
            void onBrowseButtonClicked(bool checked);

        private:
            FacetsController& facetsController;
            ProjectController& projectController;

            QHBoxLayout* layout;

            QLineEdit* lineEdit;
            QPushButton* button;

            QString fieldType;
    };
}

#endif // FILEPICKERWIDGET_H
