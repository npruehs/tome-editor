#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <random>

#include <QStringList>

#include "../Model/recordsetlist.h"


namespace Tome
{
    class FieldDefinition;
    class FieldDefinitionsController;
    class ProjectController;
    class TypesController;

    /**
     * @brief Controller for adding, updating and removing records.
     */
    class RecordsController : public QObject
    {
        Q_OBJECT

        public:
            /**
             * @brief Constructs a new controller for adding, updating and removing records.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param projectController Controller for creating, loading and saving projects.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            RecordsController(const FieldDefinitionsController& fieldDefinitionsController,
                              const ProjectController& projectController,
                              const TypesController& typesController);

            /**
             * @brief Adds a new record to the specified set.
             *
             * @exception std::out_of_range if the record set could not be found.
             * @exception std::out_of_range if the record id type of the project is invalid.
             *
             * @param id Id of the record to add.
             * @param displayName Display name of the record to add.
             * @param editorIconFieldId Id of the field to use as editor icon for the record to add.
             * @param fieldIds Ids of the fields to add to the new record.
             * @param recordSetName Name of the record set to add the new record to.
             * @return Copy of the added record.
             */
            const Record addRecord(const QVariant& id,
                                   const QString& displayName,
                                   const QString& editorIconFieldId,
                                   const QStringList& fieldIds,
                                   const QString& recordSetName);

            /**
             * @brief Adds the passed record set to the project.
             * @param recordSet Record set to add.
             */
            void addRecordSet(const RecordSet& recordSet);

            /**
             * @brief Computes an MD5 hash of all current record data.
             * @return MD5 hash of all current record data, as hex string.
             */
            const QString computeRecordsHash() const;

            /**
             * @brief Duplicats the record with the specified id, including all of its field values.
             *
             * @exception std::out_of_range if the record id type of the project is invalid.
             *
             * @param existingRecordId Id of the record to duplicate.
             * @param newRecordId Id of the record to add.
             * @return Copy of the duplicated record.
             */
            const Record duplicateRecord(const QVariant& existingRecordId, const QVariant& newRecordId);

            /**
             * @brief Gets the list of all ancestors of the record with the specified id, direct parent first.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param id Id of the record to get the ancestors of.
             * @return List of all ancestors of the record with the specified id, direct parent first.
             */
            const RecordList getAncestors(const QVariant& id) const;

            /**
             * @brief Gets the list of all records that have the record with the specified id as parent.
             * @param id Id of the record to get the children of.
             * @return List of all records that have the record with the specified id as parent.
             */
            const RecordList getChildren(const QVariant& id) const;

            /**
             * @brief Gets the list of all records that have the record with the specified id as parent, or any parent that has.
             * @param id Id of the record to get the descendants of.
             * @return List of all records that have the record with the specified id as parent, or any parent that has.
             */
            const RecordList getDescendents(const QVariant& id) const;

            /**
             * @brief Gets the value of the specified field for the record with the specified id, as inherited by its parent or any of its ancestors.
             * @param id Id of the record to get the inherited field value of.
             * @param fieldId Id of the field to get the inherited field value of.
             * @return Value of the specified field for the record with the specified id, as inherited by its parent or any of its ancestors.
             */
            const QVariant getInheritedFieldValue(const QVariant& id, const QString& fieldId) const;

            /**
             * @brief Gets the values of all fields for the record with the specified id, as inherited by its parent or any of its ancestors.
             * @param id Id of the record to get the inherited field values of.
             * @return Values of the all fields for the record with the specified id, as inherited by its parent or any of its ancestors.
             */
            const RecordFieldValueMap getInheritedFieldValues(const QVariant& id) const;

            /**
             * @brief Gets the id of the parent of the record with the specified id.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param id Id of the record to get the parent id of.
             * @return Id of the parent of the record with the specified id, or null if the record is a root of the record tree.
             */
            const QVariant getParentId(const QVariant& id) const;

            /**
             * @brief Gets a list of all record sets in the project.
             * @return List of all record sets in the project.
             */
            const RecordSetList& getRecordSets() const;

            /**
             * @brief Returns the record with the specified id.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param id Id of the record to get.
             * @return Record with the specified id.
             */
            const Record& getRecord(const QVariant& id) const;

            /**
             * @brief Gets a list of all records in the project.
             * @return List of all records in the project.
             */
            const RecordList getRecords() const;

            /**
             * @brief Gets a list of the ids of all records in the project.
             * @return List of the ids of all records in the project.
             */
            const QVariantList getRecordIds() const;

            /**
             * @brief Gets the id of the field to use as editor icon for the record with the specified id.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param id Id of the record to get the editor icon field of.
             * @return Id of the field to use as editor icon for the record with the specified id.
             */
            const QString getRecordEditorIconFieldId(const QVariant& id) const;

            /**
             * @brief Returns a list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            const QStringList getRecordNames() const;

            /**
             * @brief Gets a list of the names of all record sets in the project.
             * @return List of the names of all record sets in the project.
             */
            const QStringList getRecordSetNames() const;

            /**
             * @brief Returns the map of actual field values of the record with the specified id, including all inherited values.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param id Id of the record the get the map of field values of.
             * @return Map of actual field values of the record with the specified id.
             */
            const RecordFieldValueMap getRecordFieldValues(const QVariant& id) const;

            /**
             * @brief Gets the id of the root of the record with the specified id.
             * @param id Id of the record to get the root of.
             * @return Id of the root of the record with the specified id, or the specified id itself if the record is a root.
             */
            const QVariant getRootRecordId(const QVariant& id) const;

            /**
             * @brief Checks whether the project has a record with the specified id.
             * @param id Id of the record to look for.
             * @return true, if a record with the specified id exists, and false otherwise.
             */
            bool hasRecord(const QVariant& id) const;

            /**
             * @brief Checks whether all records with the specified ids have the same parent in the record tree.
             * @param ids List of ids of the records to check.
             * @return true, if all records with the specified ids have the same parent record, and false otherwise.
             */
            bool haveTheSameParent(const QVariantList ids) const;

            /**
             * @brief Checks whether the first record is an ancestor of the second one.
             * @param possibleAncestor Record to check if it is an ancestor.
             * @param recordId Record to check the ancestors of.
             * @return true, if the first record is an ancestor of the second one, and false otherwise.
             */
            bool isAncestorOf(const QVariant& possibleAncestor, const QVariant& recordId) const;

            /**
             * @brief Removes the record with the specified id from the project.
             * @param recordId Id of the record to remove.
             */
            void removeRecord(const QVariant& recordId);

            /**
             * @brief Removes the record set with the specified name from the project, including all records.
             * @param name Name of the record set to remove.
             */
            void removeRecordSet(const QString& name);

            /**
             * @brief Reverts all fields of the record with the specified id to their inherited or default value.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param recordId Id of the record to revert.
             */
            void revertRecord(const QVariant& recordId);

            /**
             * @brief Changes the parent of the record with the specified id.
             *
             * @throws std::out_of_range if the record to reparent could not be found.
             *
             * @see hasRecord for checking whether the record to reparent exists.
             *
             * @param recordId Id of the record to change the parent of.
             * @param newParentId Id of the new parent of the record.
             */
            void reparentRecord(const QVariant& recordId, const QVariant& newParentId);

            /**
             * @brief Marks the record with the specified id as read-only or not, preventing it from being edited, reparented or removed.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param recordId Id of the record to make read-only or not.
             * @param readOnly Whether to make the record read-only or not.
             */
            void setReadOnly(const QVariant& recordId, const bool readOnly);

            /**
             * @brief Changes the display name of the record with the specified id.
             *
             * @throws std::out_of_range if the record to rename could not be found.
             *
             * @see hasRecord for checking whether the record to rename exists.
             *
             * @param recordId Id of the record to change the display name of.
             * @param displayName New display name of the record.
             */
            void setRecordDisplayName(const QVariant& recordId, const QString& displayName);

            /**
             * @brief Changes the editor icon field id of the record with the specified id.
             *
             * @throws std::out_of_range if the record to change could not be found.
             *
             * @see hasRecord for checking whether the record to change exists.
             *
             * @param recordId Id of the record to change the editor icon field id of.
             * @param editorIconFieldId New editor icon field id of the record.
             */
            void setRecordEditorIconFieldId(const QVariant& recordId, const QString& editorIconFieldId);

            /**
             * @brief Uses the specified list of record sets as model for this controller.
             *
             * @exception std::out_of_range if the record id type of the project is invalid.
             *
             * @param model New list of record sets to use as model for this controller.
             */
            void setRecordSets(RecordSetList& model);

            /**
             * @brief Updates the properties of the specified record.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param oldId Current id of the record to update.
             * @param newId New id of the record to update.
             * @param newDisplayName New display name of the record to update.
             * @param newEditorIconFieldId New id of the field to use as editor icon for the record to update.
             * @param newFieldIds Ids of the new fields of the record to update.
             * @param newRecordSetName Name of the new record set the record should belong to.
             */
            void updateRecord(const QVariant oldId,
                              const QVariant newId,
                              const QString newDisplayName,
                              const QString newEditorIconFieldId,
                              const QStringList& newFieldIds,
                              const QString& newRecordSetName);

            /**
             * @brief Updates the current value of a record field.
             *
             * @throws std::out_of_range if the record with the specified id could not be found.
             *
             * @see hasRecord for checking whether a record with the specified id exists.
             *
             * @param recordId Id of the record to update the field value of.
             * @param fieldId Id of the field to update the value of.
             * @param fieldValue New value of the field to update.
             */
            void updateRecordFieldValue(const QVariant& recordId, const QString& fieldId, const QVariant& fieldValue);

        signals:
            /**
             * @brief Progress of the current record operation has changed.
             * @param title Title of the current record operation step.
             * @param text Details of the current record operation step.
             * @param currentValue Current progress value of the current record operation step.
             * @param maximumValue Maximum progress value of the current record operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);

            /**
             * @brief A new record has been added to the project.
             * @param recordId Id of the new record.
             * @param recordDisplayName Display name of the new record.
             * @param parentId Id of the parent of the new record, or null if its a new root of the record tree.
             */
            void recordAdded(const QVariant& recordId, const QString& recordDisplayName, const QVariant& parentId);

            /**
             * @brief A new field has been added to a record, or one of its fields has been updated or removed.
             * @param recordId Id of the record whose fields have changed.
             */
            void recordFieldsChanged(const QVariant& recordId);

            /**
             * @brief A record has been removed from the project.
             * @param recordId Id of the record that has been removed from the project.
             */
            void recordRemoved(const QVariant& recordId);

            /**
             * @brief The parent of a record has changed.
             * @param recordId Id of the record whose parent has changed.
             * @param oldParentId Id of the previous parent of the record.
             * @param newParentId Id of the new parent of the record, or null if its a new root of the record tree.
             */
            void recordReparented(const QVariant& recordId, const QVariant& oldParentId, const QVariant& newParentId);

            /**
             * @brief The properties of a record have been updated.
             * @param oldId Previous id of the record which has been updated.
             * @param oldDisplayName Previous display name of the record which has been updated.
             * @param oldEditorIconFieldId Previous id of the field to use as editor icon for the record which has been updated.
             * @param newId New id of the record which has been updated.
             * @param newDisplayName New display name of the record which has been updated.
             * @param newEditorIconFieldId New id of the field to use as editor icon for the record which has been updated.
             */
            void recordUpdated(const QVariant& oldId,
                               const QString& oldDisplayName,
                               const QString& oldEditorIconFieldId,
                               const QVariant& newId,
                               const QString& newDisplayName,
                               const QString& newEditorIconFieldId);

            /**
             * @brief A record set has been added to or removed from the project.
             */
            void recordSetsChanged();

        private slots:
            void onFieldAdded(const Tome::FieldDefinition& fieldDefinition);
            void onFieldRemoved(const Tome::FieldDefinition& fieldDefinition);
            void onFieldUpdated(const Tome::FieldDefinition& oldFieldDefinition, const Tome::FieldDefinition& newFieldDefinition);

        private:
            RecordSetList* model;

            const FieldDefinitionsController& fieldDefinitionsController;
            const ProjectController& projectController;
            const TypesController& typesController;

            std::mt19937 recordIdGenerator;
            std::uniform_int_distribution<int> recordIdDistribution;

            void addRecordField(const QVariant& recordId, const QString& fieldId);
            int generateIntegerId();
            const QString generateUuid() const;
            Record* getRecordById(const QVariant& id) const;
            void moveFieldToComponent(const QString& fieldId, const QString& oldComponent, const QString& newComponent);
            void moveRecordToSet(const QVariant& recordId, const QString& recordSetName);
            void removeRecordField(const QVariant& recordId, const QString& fieldId);
            void renameRecordField(const QString oldFieldId, const QString newFieldId);
            QVariant revertFieldValue(const QVariant& recordId, const QString& fieldId);
            void updateRecordReferences(const QVariant oldReference, const QVariant newReference);
            void verifyRecordIds();
            void verifyRecordIntegerIds();
            void verifyRecordStringIds();
            void verifyRecordUuids();
    };
}

#endif // RECORDSCONTROLLER_H
