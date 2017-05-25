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

/**
 * @brief Window for adding a new derived type or editing an existing one.
 */
class DerivedTypeWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new derived type or editing an existing one.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param facetsController Controller for validating type facets.
         * @param recordsController Controller for adding, updating and removing records.
         * @param parent Optional owner widget.
         */
        explicit DerivedTypeWindow(
                Tome::TypesController& typesController,
                Tome::FacetsController& facetsController,
                Tome::RecordsController& recordsController,
                QWidget *parent = 0);
        ~DerivedTypeWindow();

        /**
         * @brief Gets the name of the type the type is derived from.
         * @return Name of the type the type is derived from.
         */
        QString getBaseType() const;

        /**
         * @brief Gets the values of all constraining facets of the type.
         * @return Values of all constraining facets of the type.
         */
        QVariantMap getFacets() const;

        /**
         * @brief Gets the name of the type.
         * @return Name of the type.
         */
        QString getTypeName() const;

        /**
         * @brief Gets the name of the set the custom type should belong to.
         * @return Name of the set the custom type should belong to.
         */
        QString getTypeSetName() const;

        /**
         * @brief Fills all combo boxes of this window with current data.
         */
        void init();

        /**
         * @brief Sets the name of the type the type is derived from.
         * @param baseType Name of the type the type is derived from.
         */
        void setBaseType(const QString& baseType) const;

        /**
         * @brief Sets the values of all constraining facets of the type.
         * @param facets Values of all constraining facets of the type.
         */
        void setFacets(const QVariantMap& facets);

        /**
         * @brief Sets the name of the type.
         * @param typeName Name of the type.
         */
        void setTypeName(const QString& typeName);

        /**
         * @brief Sets the name of the set the custom type should belong to.
         * @param typeSetName Name of the set the custom type should belong to.
         */
        void setTypeSetName(const QString& typeSetName);

        /**
         * @brief Sets the list of names of available custom type sets.
         * @param typeSetNames List of names of available custom type sets.
         */
        void setTypeSetNames(const QStringList& typeSetNames);

    public slots:
        /**
         * @brief Validates all data and closes this window if successful.
         */
        void accept();

    protected:
        /**
         * @brief Sets up this window, e.g. by setting the focus on the first widget.
         * @param event Event for showing this window.
         */
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

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
