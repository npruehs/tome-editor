#ifndef ENUMERATIONWINDOW_H
#define ENUMERATIONWINDOW_H

#include <QDialog>

class EnumerationMemberWindow;

namespace Ui {
    class EnumerationWindow;
}

namespace Tome
{
    class CustomType;
}

/**
 * @brief Window for adding a new enumeration or editing an existing one.
 */
class EnumerationWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new enumeration or editing an existing one.
         * @param parent Optional owner widget.
         */
        explicit EnumerationWindow(QWidget *parent = 0);
        ~EnumerationWindow();

        /**
         * @brief Gets the name of the enumeration.
         * @return Name of the enumeration.
         */
        QString getEnumerationName() const;

        /**
         * @brief Gets the items of the enumeration.
         * @return Items of the enumeration.
         */
        QStringList getEnumerationMembers() const;

        /**
         * @brief Gets the name of the set the enumeration should belong to.
         * @return Name of the set the enumeration should belong to.
         */
        QString getTypeSetName() const;

        /**
         * @brief Sets the name of the enumeration.
         * @param typeName Name of the enumeration.
         */
        void setEnumerationName(const QString& typeName);

        /**
         * @brief Sets the items of the enumeration.
         * @param enumeration Items of the enumeration.
         */
        void setEnumerationMembers(const QStringList enumeration);

        /**
         * @brief Sets the name of the set the enumeration should belong to.
         * @param typeSet Name of the set the enumeration should belong to.
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

    private slots:
        void on_actionNew_Member_triggered();
        void on_actionDelete_Member_triggered();

    private:
        Ui::EnumerationWindow *ui;

        EnumerationMemberWindow* enumerationMemberWindow;

        QStringList enumeration;

        bool validate();
};

#endif // ENUMERATIONWINDOW_H
