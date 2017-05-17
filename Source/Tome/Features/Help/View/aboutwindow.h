#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

namespace Ui {
    class AboutWindow;
}

/**
 * @brief Window for showing copyright and license information about Tome.
 */
class AboutWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for showing copyright and license information about Tome.
         * @param parent Optional owner widget.
         */
        explicit AboutWindow(QWidget *parent = 0);
        ~AboutWindow();

    private:
        Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
