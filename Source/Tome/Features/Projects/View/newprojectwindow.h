#ifndef NEWPROJECTWINDOW_H
#define NEWPROJECTWINDOW_H

#include <QDialog>
#include <QString>

#include "../Model/recordidtype.h"

namespace Ui {
    class NewProjectWindow;
}

/**
 * @brief Window for creating a new Tome project.
 */
class NewProjectWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for creating a new Tome project.
         * @param parent Optional owner widget.
         */
        explicit NewProjectWindow(QWidget *parent = 0);
        ~NewProjectWindow();

        /**
         * @brief Gets the name of the new project, as shown in the UI.
         * @return Name of the new project.
         */
        QString getProjectName() const;

        /**
         * @brief Gets the absolute path to the new project, without file name.
         * @return Absolute path to the new project, without file name.
         */
        QString getProjectPath() const;

        /**
         * @brief Gets the type of the ids of the records of the new project.
         * @return Type of the ids of the records of the new project.
         */
        Tome::RecordIdType::RecordIdType getProjectRecordIdType() const;

    private slots:
        void on_comboBoxRecordIdType_currentTextChanged(const QString &text);
        void on_pushButtonBrowse_clicked();

    private:
        Ui::NewProjectWindow *ui;
};

#endif // NEWPROJECTWINDOW_H
