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

    /**
     * @brief Widget that allows to browse for a file and shows the current path.
     */
    class FilePickerWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget that allows to browse for a file and shows the current path.
             * @param facetsController Controller for validating type facets.
             * @param projectController Controller for creating, loading and saving projects.
             * @param parent Optional owner widget.
             */
            explicit FilePickerWidget(FacetsController& facetsController, ProjectController& projectController, QWidget* parent = 0);
            ~FilePickerWidget();

            /**
             * @brief Gets the current file name.
             * @return Current file name.
             */
            QVariant getFileName() const;

            /**
             * @brief Sets the current file name.
             * @param fileName Current file name.
             */
            void setFileName(const QVariant& fileName);

            /**
             * @brief Sets the type of the field to edit with this widget (e.g. custom type deriving from File).
             * @param fieldType Type of the field to edit with this widget
             */
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
