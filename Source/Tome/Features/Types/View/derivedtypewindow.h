#ifndef DERIVEDTYPEWINDOW_H
#define DERIVEDTYPEWINDOW_H

#include <QDialog>

namespace Ui {
    class DerivedTypeWindow;
}

namespace Tome
{
    class TypesController;
    class FacetsController;
    class RecordsController;
}

class DerivedTypeWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit DerivedTypeWindow(
                Tome::TypesController& typesController,
                Tome::FacetsController& facetsController,
                Tome::RecordsController& recordsController,
                QWidget *parent = 0);
        ~DerivedTypeWindow();

        QString getBaseType() const;
        QVariantMap getFacets() const;
        QString getTypeName() const;
        QString getTypeSetName() const;

        void init();

        void setBaseType(const QString& baseType) const;
        void setFacets(const QVariantMap& facets);
        void setTypeName(const QString& typeName);
        void setTypeSetName(const QString& typeSetName);
        void setTypeSetNames(const QStringList& typeSetNames);

    public slots:
        void accept();

    private slots:
        void on_comboBoxBaseType_currentIndexChanged(const QString &arg1);

    private:
        static const int FacetFormRow;

        Ui::DerivedTypeWindow *ui;

        Tome::TypesController& typesController;
        Tome::FacetsController& facetsController;
        Tome::RecordsController& recordsController;

        QList<QWidget*> facetWidgets;

        bool validate();
};

#endif // DERIVEDTYPEWINDOW_H
