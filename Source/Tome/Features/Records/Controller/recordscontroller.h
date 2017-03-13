#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QStringList>

#include "../Model/recordsetlist.h"


namespace Tome
{
    class FieldDefinition;
    class FieldDefinitionsController;
    class ProjectController;
    class TypesController;

    class RecordsController : public QObject
    {
        Q_OBJECT

        public:
            RecordsController(const FieldDefinitionsController& fieldDefinitionsController,
                              const ProjectController& projectController,
                              const TypesController& typesController);

            const Record addRecord(const QVariant& id,
                                   const QString& displayName,
                                   const QString& editorIconFieldId,
                                   const QStringList& fieldIds,
                                   const QString& recordSetName);
            void addRecordSet(const RecordSet& recordSet);

            const QString computeRecordsHash() const;

            const Record duplicateRecord(const QVariant& existingRecordId, const QVariant& newRecordId);

            /**
             * @brief getAncestors Gets the list of all ancestors of the record with the specified id, direct parent first.
             * @param id Id of the record to get the ancestors of.
             * @return List of all ancestors of the record with the specified id, direct parent first.
             */
            const RecordList getAncestors(const QVariant& id) const;

            const RecordList getChildren(const QVariant& id) const;
            const RecordList getDescendents(const QVariant& id) const;
            const QVariant getInheritedFieldValue(const QVariant& id, const QString& fieldId) const;
            const RecordFieldValueMap getInheritedFieldValues(const QVariant& id) const;
            const QVariant getParentId(const QVariant& id) const;

            const RecordSetList& getRecordSets() const;

            /**
             * @brief getRecord Returns the record with the specified id.
             * @param id Id of the record to get.
             * @return Record with the specified id.
             */
            const Record& getRecord(const QVariant& id) const;

            const RecordList getRecords() const;

            const QVariantList getRecordIds() const;

            const QString getRecordEditorIconFieldId(const QVariant& id) const;

            /**
             * @brief getRecordNames Returns the list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            const QStringList getRecordNames() const;

            const QStringList getRecordSetNames() const;

            /**
             * @brief getRecordFieldValues Returns the map of actual field values of the record with the specified id, including all inherited values.
             * @param id Id of the record the get the map of field values of.
             * @return Map of actual field values of the record with the specified id.
             */
            const RecordFieldValueMap getRecordFieldValues(const QVariant& id) const;

            const QVariant getRootRecordId(const QVariant& id) const;

            bool hasRecord(const QVariant& id) const;
            bool haveTheSameParent(const QVariantList ids) const;
            int indexOf(const Record& record) const;

            /**
             * @brief isAncestorOf Checks whether the first record is an ancestor of the second one.
             * @param possibleAncestor Record to check if it is an ancestor.
             * @param recordId Record to check the ancestors of.
             * @return true, if the first record is an ancestor of the second one, and false otherwise.
             */
            bool isAncestorOf(const QVariant& possibleAncestor, const QVariant& recordId) const;

            void removeRecord(const QVariant& recordId);
            void removeRecordSet(const QString& name);

            void revertRecord(const QVariant& recordId);

            void reparentRecord(const QVariant& recordId, const QVariant& newParentId);
            void setReadOnly(const QVariant& recordId, const bool readOnly);
            void setRecordSets(RecordSetList& model);
            void updateRecord(const QVariant oldId,
                              const QVariant newId,
                              const QString newDisplayName,
                              const QString newEditorIconFieldId,
                              const QStringList& fieldIds,
                              const QString& recordSetName);
            void updateRecordFieldValue(const QVariant& recordId, const QString& fieldId, const QVariant& fieldValue);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);
            void recordAdded(const QVariant& recordId, const QString& recordDisplayName, const QVariant& parentId);
            void recordFieldsChanged(const QVariant& recordId);
            void recordRemoved(const QVariant& recordId);
            void recordReparented(const QVariant& recordId, const QVariant& oldParentId, const QVariant& newParentId);
            void recordUpdated(const QVariant& oldId,
                               const QString& oldDisplayName,
                               const QString& oldEditorIconFieldId,
                               const QVariant& newId,
                               const QString& newDisplayName,
                               const QString& newEditorIconFieldId);
            void recordSetsChanged();

        private slots:
            void onFieldAdded(const Tome::FieldDefinition& fieldDefinition);
            void onFieldRemoved(const Tome::FieldDefinition& fieldDefinition);
            void onFieldUpdated(const Tome::FieldDefinition& oldFieldDefinition, const Tome::FieldDefinition& newFieldDefinition);

        private:
            RecordSetList* model;
            qlonglong nextRecordIntegerId;

            const FieldDefinitionsController& fieldDefinitionsController;
            const ProjectController& projectController;
            const TypesController& typesController;

            void addRecordField(const QVariant& recordId, const QString& fieldId);
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
