#ifndef COMPONENTWINDOW_H
#define COMPONENTWINDOW_H

#include <QDialog>

namespace Ui {
    class ComponentWindow;
}

/**
 * @brief Window for adding a new component.
 */
class ComponentWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new component.
         * @param parent Optional owner widget.
         */
        explicit ComponentWindow(QWidget *parent = 0);
        ~ComponentWindow();

        /**
         * @brief Gets the name of the component to add.
         * @return Name of the component to add.
         */
        QString getComponentName() const;

        /**
         * @brief Gets the name of the component set to add the new component to.
         * @return Name of the component set to add the new component to.
         */
        QString getComponentSetName() const;

        /**
         * @brief Sets the list of component names that are already taken and thus forbidden.
         * @param disallowedComponentIds List of component names that are forbidden.
         */
        void setDisallowedComponentIds(const QStringList disallowedComponentIds);

        /**
         * @brief Pre-selects a component set to add the new component to.
         * @param componentSetName Component set to add the new component to.
         */
        void setComponentSetName(const QString& componentSetName);

        /**
         * @brief Sets the list of available component set names.
         * @param componentSetNames List of available component set names.
         */
        void setComponentSetNames(const QStringList& componentSetNames);

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
        Ui::ComponentWindow *ui;

        QStringList disallowedComponentIds;

        bool validate();
};

#endif // COMPONENTWINDOW_H
