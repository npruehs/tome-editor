#ifndef FIELDDEFINITIONSCONTROLLER_H
#define FIELDDEFINITIONSCONTROLLER_H

#include "../Model/fielddefinitionsetlist.h"


namespace Tome
{
    class FieldDefinitionsController
    {
        public:
            FieldDefinitionsController();

            const FieldDefinition addFieldDefinition(const QString& id, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& component, const QString& description);
            void addFieldDefinitionSet(const FieldDefinitionSet& fieldDefinitionSet);
            const FieldDefinition& getFieldDefinition(const QString& id) const;
            const FieldDefinitionList getFieldDefinitions() const;
            const FieldDefinitionSetList& getFieldDefinitionSets() const;
            const QStringList getFieldDefinitionSetNames() const;
            bool hasFieldDefinition(const QString& id) const;
            int indexOf(const FieldDefinition& fieldDefinition) const;
            void moveFieldDefinitionToSet(const QString& fieldDefinitionId, const QString& fieldDefinitionSetName);
            void removeFieldComponent(const QString componentName);
            void removeFieldDefinition(const QString& fieldId);
            void removeFieldDefinitionSet(const QString& name);
            void renameFieldType(const QString oldTypeName, const QString newTypeName);
            void setFieldDefinitionSets(FieldDefinitionSetList& model);
            void updateFieldDefinition(
                    const QString oldId,
                    const QString newId,
                    const QString& displayName,
                    const QString& fieldType,
                    const QVariant& defaultValue,
                    const QString& component,
                    const QString& description,
                    const QString& fieldDefinitionSetName);

        private:
            FieldDefinitionSetList* model;

            FieldDefinition* getFieldDefinitionById(const QString& id) const;
    };
}

#endif // FIELDDEFINITIONSCONTROLLER_H
