#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <QDialog>

#include "../Model/recordfieldstate.h"
#include "../../Fields/Model/fielddefinitionlist.h"
#include "../../Components/Model/componentlist.h"
#include "../../Projects/Model/recordidtype.h"
#include "../../Records/Model/recordfieldvaluemap.h"
#include "../../Records/Controller/recordnamevalidator.h"

namespace Ui {
    class RecordWindow;
}

/**
 * @brief Window for adding a new record or editing an existing one.
 */
class RecordWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new record or editing an existing one.
         * @param parent Optional owner widget.
         */
        explicit RecordWindow(QWidget *parent = 0);
        ~RecordWindow();

        /**
         * @brief Gets the display name of the record.
         * @return Display name of the record.
         */
        QString getRecordDisplayName() const;

        /**
         * @brief Gets the id of the field to use as editor icon for the record.
         * @return Id of the field to use as editor icon for the record.
         */
        QString getRecordEditorIconFieldId() const;

        /**
         * @brief Gets the unique id of the record.
         * @return Unique id of the record.
         */
        QVariant getRecordId() const;

        /**
         * @brief Gets which fields are used by the record.
         * @return Which fields are used by the record.
         */
        QMap<QString, Tome::RecordFieldState::RecordFieldState> getRecordFields() const;

        /**
         * @brief Gets the name of the set the record belongs to.
         * @return Name of the set the record belongs to.
         */
        QString getRecordSetName() const;

        /**
         * @brief Sets the list of record ids that are prohibited (e.g. because other records with the same id exist).
         * @param disallowedRecordIds List of record ids that are prohibited.
         */
        void setDisallowedRecordIds(const QVariantList disallowedRecordIds);

        /**
         * @brief Sets the display name of the record.
         * @param displayName Display name of the record.
         */
        void setRecordDisplayName(const QString& displayName);

        /**
         * @brief Sets the unique id of the record.
         * @param id Unique id of the record.
         */
        void setRecordId(const QVariant& id);

        /**
         * @brief Sets whether to automatically update the record id to reflect the display name, or not.
         * @param recordIdLocked Whether to automatically update the record id to reflect the display name, or not.
         */
        void setRecordIdLocked(const bool recordIdLocked);

        /**
         * @brief Sets the type of the id of the record.
         * @param recordIdType Type of the id of the record.
         */
        void setRecordIdType(const Tome::RecordIdType::RecordIdType recordIdType);

        /**
         * @brief Sets the id of the field to use as editor icon for the record.
         * @param editorIconFieldId Id of the field to use as editor icon for the record.
         */
        void setRecordEditorIconFieldId(const QString& editorIconFieldId);

        /**
         * @brief Sets the list of available record fields, initially disabling all of them.
         * @param fieldDefinitions List of available record fields.
         */
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions);

        /**
         * @brief Sets the list of fields used by the record.
         * @param fieldDefinitions List of available record fields.
         * @param componentDefinitions  List of available components.
         * @param ownFieldValues Fields used by the record.
         * @param inheritedFieldValues Fields used by ancestors of the record.
         */
        void setRecordFields(const Tome::FieldDefinitionList& fieldDefinitions,
                             const Tome::ComponentList &componentDefinitions,
                             const Tome::RecordFieldValueMap& ownFieldValues,
                             const Tome::RecordFieldValueMap& inheritedFieldValues);

        /**
         * @brief Sets the list of available components.
         * @param components List of available components.
         */
        void setRecordComponents(const Tome::ComponentList& components);

        /**
         * @brief Sets the name of the set the record belongs to.
         * @param recordSetName Name of the new set the record belongs to.
         */
        void setRecordSetName(const QString& recordSetName);

        /**
         * @brief Sets the list of available record sets.
         * @param recordSetNames List of available record sets.
         */
        void setRecordSetNames(const QStringList& recordSetNames);

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

        QVariantList disallowedRecordIds;
        Tome::RecordNameValidator nameValidator;

        // Whether to automatically update the record id to reflect the display name, or not.
        bool recordIdLocked;

        QVariant recordId;
        Tome::RecordIdType::RecordIdType recordIdType;

        void clearRecordComponents();
        void clearRecordFields();
        void setRecordComponent(const QString& componentId, const Tome::RecordFieldState::RecordFieldState state);
        void setRecordField(const QString& fieldId, const QString& fieldComponent, const Tome::RecordFieldState::RecordFieldState state);

        bool validate();
};

#endif // RECORDWINDOW_H
