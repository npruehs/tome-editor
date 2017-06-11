#ifndef ENUMERATIONMEMBERWINDOW_H
#define ENUMERATIONMEMBERWINDOW_H

#include <QDialog>

namespace Ui {
    class EnumerationMemberWindow;
}

/**
 * @brief Window for adding a new item to an enumeration.
 */
class EnumerationMemberWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new item to an enumeration.
         * @param parent Optional owner widget.
         */
        explicit EnumerationMemberWindow(QWidget *parent = 0);
        ~EnumerationMemberWindow();

        /**
         * @brief Gets the item to add.
         * @return Item to add.
         */
        QString getText() const;

    public slots:
        /**
         * @brief Validates all data and closes this window if successful.
         */
        void accept() Q_DECL_OVERRIDE;

    protected:
        /**
         * @brief Sets up this window, e.g. by setting the focus on the first widget.
         * @param event Event for showing this window.
         */
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private:
        Ui::EnumerationMemberWindow *ui;

        bool validate();
};

#endif // ENUMERATIONMEMBERWINDOW_H
