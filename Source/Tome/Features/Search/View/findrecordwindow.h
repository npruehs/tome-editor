#ifndef FINDRECORDWINDOW_H
#define FINDRECORDWINDOW_H

#include <QDialog>

namespace Ui {
    class FindRecordWindow;
}

/**
 * @brief Window for finding specific records.
 */
class FindRecordWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for finding specific records.
         * @param parent Optional owner widget.
         */
        explicit FindRecordWindow(QWidget *parent = 0);
        ~FindRecordWindow();

        /**
         * @brief Gets the record id or display name search for.
         * @return Record id or display name search for.
         */
        QString getSearchPattern() const;

    private:
        Ui::FindRecordWindow *ui;
};

#endif // FINDRECORDWINDOW_H
