#ifndef DUPLICATERECORDWINDOW_H
#define DUPLICATERECORDWINDOW_H

#include "../../Records/Controller/recordnamevalidator.h"

#include <QDialog>

namespace Ui {
    class DuplicateRecordWindow;
}

/**
 * @brief Window for duplicating an existing record, including all of its field values.
 */
class DuplicateRecordWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for duplicating an existing record, including all of its field values.
         * @param parent Optional owner widget.
         */
        explicit DuplicateRecordWindow(QWidget *parent = 0);
        ~DuplicateRecordWindow();

        /**
         * @brief Gets the id of the record to add.
         * @return Id of the record to add.
         */
        QString getRecordId() const;

        /**
         * @brief Sets the list of record ids that are prohibited (e.g. because other records with the same id exist).
         * @param disallowedRecordIds List of record ids that are prohibited.
         */
        void setDisallowedRecordIds(const QStringList disallowedRecordIds);

        /**
         * @brief Sets the id of the record to add.
         * @param id Id of the record to add.
         */
        void setRecordId(const QString& id);

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
        void showEvent(QShowEvent* event);

    private:
        Ui::DuplicateRecordWindow *ui;

        QStringList disallowedRecordIds;
        Tome::RecordNameValidator nameValidator;

        bool validate();
};

#endif // DUPLICATERECORDWINDOW_H
