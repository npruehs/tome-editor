#ifndef FIELDVALUEWIDGET_H
#define FIELDVALUEWIDGET_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

namespace Tome
{
    class CustomType;
    class FilePickerWidget;
    class ListWidget;
    class MapWidget;
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;
    class Vector2IWidget;
    class Vector2RWidget;
    class Vector3IWidget;
    class Vector3RWidget;
    class Facet;

    /**
     * @brief Changes its appearance depending on the specified type.
     */
    class FieldValueWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget that changes its appearance depending on the specified type.
             * @param facetsController Controller for validating type facets.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param parent Optional owner widget.
             */
            explicit FieldValueWidget(FacetsController& facetsController,
                                      ProjectController& projectController,
                                      RecordsController& recordsController,
                                      TypesController& typesController,
                                      QWidget* parent = 0);
            ~FieldValueWidget();

            /**
             * @brief Gets the current type of the values changed by this widget.
             * @return Current type of the values changed by this widget.
             */
            QString getFieldType() const;

            /**
             * @brief Gets the current value of this widget.
             *
             * @exception std::runtime_error if the current field type is unknown.
             *
             * @return Current value of this widget.
             */
            QVariant getFieldValue() const;

            /**
             * @brief Sets the current type of the values changed by this widget.
             *
             * @exception std::runtime_error if the field type is unknown.
             *
             * @param fieldType Current type of the values changed by this widget.
             */
            void setFieldType(const QString& fieldType);

            /**
             * @brief Sets the current value of this widget.
             *
             * @exception std::runtime_error if the current field type is unknown.
             *
             * @param fieldValue Current value of this widget.
             */
            void setFieldValue(const QVariant& fieldValue);

            /**
             * @brief Validates the current value against all facets of its type.
             * @return Translated error message, if any validation errors occur, and an empty string otherwise.
             */
            QString validate();

        protected:
            /**
             * @brief Sets the focus to the current widget, if any, and pre-selects any content.
             * @param event Event for focusing this widget.
             */
            virtual void focusInEvent(QFocusEvent* event) Q_DECL_OVERRIDE;

    private slots:
            void onColorDialogCurrentColorChanged(const QColor& color);
            void onDoubleSpinBoxValueChanged(double d);
            void onPlainTextEditTextChanged();
            void onComboBoxCurrentIndexChanged(const QString& text);
            void onSpinBoxValueChanged(int i);

        private:
            QWidget* currentWidget;
            QString fieldType;

            QCheckBox* checkBox;
            QColorDialog* colorDialog;
            QDoubleSpinBox* doubleSpinBox;
            FilePickerWidget* filePicker;
            QVBoxLayout* layout;
            QPlainTextEdit* plainTextEdit;
            ListWidget* listWidget;
            MapWidget* mapWidget;
            QComboBox* comboBox;
            QSpinBox* spinBox;
            Vector2IWidget* vector2IWidget;
            Vector3IWidget* vector3IWidget;
            Vector2RWidget* vector2RWidget;
            Vector3RWidget* vector3RWidget;

            QLabel* errorLabel;

            FacetsController& facetsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            TypesController& typesController;

            void addWidget(QWidget* widget);
            QVariant getFieldValueForType(const QString& typeName) const;
            void selectWidgetForType(const QString& typeName);
            void setCurrentWidget(QWidget* widget);
            void setEnumeration(const QStringList& enumeration);
            void setFieldValueForType(const QVariant& fieldValue, const QString& typeName);
            void updateErrorLabel();
    };
}

#endif // FIELDVALUEWIDGET_H
