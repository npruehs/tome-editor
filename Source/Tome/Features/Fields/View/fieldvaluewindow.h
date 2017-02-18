#ifndef FIELDVALUEWINDOW_H
#define FIELDVALUEWINDOW_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
    class FieldValueWindow;
}

namespace Tome
{
    class CustomType;
    class Facet;
    class FieldValueWidget;
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

class FieldValueWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldValueWindow(Tome::FacetsController& facetsController,
                                  Tome::ProjectController& projectController,
                                  Tome::RecordsController& recordsController,
                                  Tome::TypesController& typesController,
                                  QWidget *parent = 0);
        ~FieldValueWindow();

        QVariant getFieldValue() const;

        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldValue(const QVariant& fieldValue);
        void setFieldType(const QString& fieldType) const;

    public slots:
        void accept();

    signals:
        void revert();

    protected:
        virtual void showEvent(QShowEvent* event);

    private slots:
        void on_toolButtonRevert_clicked();

    private:
        Ui::FieldValueWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::FacetsController& facetsController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // FIELDVALUEWINDOW_H
