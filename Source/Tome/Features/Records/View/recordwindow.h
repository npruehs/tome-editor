#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <QDialog>

#include "../Model/recordfieldstate.h"
#include "../../Fields/Model/fielddefinitionlist.h"
#include "../../Components/Model/componentlist.h"
#include "../../Records/Model/recordfieldvaluemap.h"
#include "../../Records/Controller/recordnamevalidator.h"

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
        QString getRecordEditorIconFieldId() const;
        QString getRecordId() const;
        QMap<QString, Tome::RecordFieldState::RecordFieldState> getRecordFields() const;
        QString getRecordSetName() const;

        void clearRecordFields();

        void setDisallowedRecordIds(const QStringList disallowedRecordIds);
        void setRecordDisplayName(const QString& displayName);
        void setRecordId(const QString& id);
        void setRecordIdLocked(const bool recordIdLocked);
        void setRecordIntegerId(const long id);
        void setRecordEditorIconFieldId(const QString& editorIconFieldId);
        void setRecordField(const QString& fieldId, const QString& fieldComponent, const Tome::RecordFieldState::RecordFieldState state);
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions);
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions, const Tome::ComponentList &componentDefinitions, const Tome::RecordFieldValueMap& ownFieldValues, const Tome::RecordFieldValueMap& inheritedFieldValues);
        void setRecordComponent(const QString& componentId, const Tome::RecordFieldState::RecordFieldState state);
        void setRecordComponents(const Tome::ComponentList& components);
        void setRecordSetName(const QString& recordSetName);
        void setRecordSetNames(const QStringList& recordSetNames);
        void setRecordUuid(const QString& uuid);
        void clearRecordComponents();

    public slots:
        void accept();

    protected:
        void showEvent(QShowEvent* event);

    private slots:
        void on_lineEditDisplayName_textEdited(const QString &arg1);
        void on_lineEditId_textEdited(const QString &arg1);

        void onCheckBoxStateChanged(int state);
        void onComponentCheckBoxStateChanged(int state);

    private:
        static const QString PropertyFieldComponent;
        static const QString PropertyFieldId;
        static const QString PropertyComponentId;

        Ui::RecordWindow *ui;

        QStringList disallowedRecordIds;
        RecordNameValidator nameValidator;

        // Whether to automatically update the record id to reflect the display name, or not.
        bool recordIdLocked;

        bool validate();
};

#endif // RECORDWINDOW_H
