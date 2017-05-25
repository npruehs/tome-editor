#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QDialog>


namespace Ui {
    class ListWindow;
}

namespace Tome
{
    class TypesController;
}

/**
 * @brief Window for adding a new list type or editing an existing one.
 */
class ListWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new list type or editing an existing one.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit ListWindow(Tome::TypesController& typesController, QWidget *parent = 0);
        ~ListWindow();

        /**
         * @brief Gets the name of the list type.
         * @return Name of the list type.
         */
        QString getListName() const;

        /**
         * @brief Gets the type of the items of the list.
         * @return Type of the items of the list.
         */
        QString getListItemType() const;

        /**
         * @brief Gets the name of the set the list type should belong to.
         * @return Name of the set the list type should belong to.
         */
        QString getTypeSetName() const;

        /**
         * @brief Fills all combo boxes of this window with current data.
         */
        void init();

        /**
         * @brief Sets the name of the list type.
         * @param listName Name of the list type.
         */
        void setListName(const QString& listName);

        /**
         * @brief Sets the type of the items of the list.
         * @param itemType Type of the items of the list.
         */
        void setListItemType(const QString& itemType);

        /**
         * @brief Sets the name of the set the list type should belong to.
         * @param typeSet Name of the set the list type should belong to.
         */
        void setTypeSetName(const QString& typeSet);

        /**
         * @brief Sets the list of names of available custom type sets.
         * @param typeSets List of names of available custom type sets.
         */
        void setTypeSetNames(const QStringList& typeSets);

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

    private:
        Ui::ListWindow *ui;
        Tome::TypesController& typesController;

        bool validate();
};

#endif // LISTWINDOW_H
