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
            const FieldDefinition& getFieldDefinition(const QString& id) const;
            const FieldDefinitionList getFieldDefinitions() const;
            bool hasFieldDefinition(const QString& id) const;
            int indexOf(const FieldDefinition& fieldDefinition) const;
            void removeFieldComponent(QString componentName);
            void removeFieldDefinition(QString& fieldId);
            void setFieldDefinitionSets(FieldDefinitionSetList& model);
            void updateFieldDefinition(const QString oldId, const QString newId, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& component, const QString& description);

        private:
            FieldDefinitionSetList* model;

            FieldDefinition* getFieldDefinitionById(const QString& id) const;
    };
}

#endif // FIELDDEFINITIONSCONTROLLER_H
