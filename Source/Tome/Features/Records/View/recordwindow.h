#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <QDialog>

#include "../Model/recordfieldstate.h"
#include "../../Fields/Model/fielddefinitionlist.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Ui {
    class RecordWindow;
}

class RecordWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit RecordWindow(QWidget *parent = 0);
        ~RecordWindow();

        QString getRecordDisplayName() const;
        QString getRecordId() const;
        QMap<QString, Tome::RecordFieldState::RecordFieldState> getRecordFields() const;

        void clearRecordFields();

        void setRecordDisplayName(const QString& displayName);
        void setRecordId(const QString& id);
        void setRecordField(const QString& fieldId, const QString& fieldComponent, const Tome::RecordFieldState::RecordFieldState state);
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions);
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions, const Tome::RecordFieldValueMap& ownFieldValues, const Tome::RecordFieldValueMap& inheritedFieldValues);

    public slots:
        void accept();

    private slots:
        void on_lineEditDisplayName_textEdited(const QString &arg1);

        void onCheckBoxStateChanged(int state);

    private:
        static const QString PropertyFieldComponent;
        static const QString PropertyFieldId;

        Ui::RecordWindow *ui;

        bool validate();
};

#endif // RECORDWINDOW_H
