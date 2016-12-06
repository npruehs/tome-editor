#ifndef FIELDDEFINITIONSCONTROLLER_H
#define FIELDDEFINITIONSCONTROLLER_H

#include "../Model/fielddefinitionsetlist.h"
#include "../../Components/Model/component.h"

namespace Tome
{
    class ComponentsController;

    class FieldDefinitionsController : public QObject
    {
            Q_OBJECT

        public:
            FieldDefinitionsController(const ComponentsController& componentsController);

            const FieldDefinition addFieldDefinition(
                    const QString& id,
                    const QString& displayName,
                    const QString& fieldType,
                    const QVariant& defaultValue,
                    const QString& component,
                    const QString& description,
                    const QString& fieldDefinitionSetName);
            void addFieldDefinitionSet(const FieldDefinitionSet& fieldDefinitionSet);
            const FieldDefinition& getFieldDefinition(const QString& id) const;
            const FieldDefinitionList getFieldDefinitions() const;
            const FieldDefinitionList getFieldDefinitionsOfComponent(const QString& component) const;
            const FieldDefinitionSetList& getFieldDefinitionSets() const;
            const QStringList getFieldDefinitionSetNames() const;
            bool hasFieldDefinition(const QString& id) const;
            int indexOf(const FieldDefinition& fieldDefinition) const;
            void removeFieldDefinition(const QString& fieldId);
            void removeFieldDefinitionSet(const QString& name);
            void renameFieldType(const QString oldTypeName, const QString newTypeName);
            void setFieldDefinitionSets(FieldDefinitionSetList& model);
            void updateFieldDefinition(const QString oldId,
                    const QString newId,
                    const QString& displayName,
                    const QString& fieldType,
                    const QVariant& defaultValue,
                    const QString& component,
                    const QString& description,
                    const QString& fieldDefinitionSetName);

        signals:
            void fieldDefinitionAdded(const Tome::FieldDefinition& fieldDefinition);
            void fieldDefinitionRemoved(const Tome::FieldDefinition& fieldDefinition);
            void fieldDefinitionUpdated(const Tome::FieldDefinition& oldFieldDefinition, const Tome::FieldDefinition& newFieldDefinition);

        private slots:
            void onComponentRemoved(const Tome::Component& component);

        private:
            const ComponentsController& componentsController;

            FieldDefinitionSetList* model;

            FieldDefinition* getFieldDefinitionById(const QString& id) const;
            void moveFieldDefinitionToSet(const QString& fieldDefinitionId, const QString& fieldDefinitionSetName);

    };
}

#endif // FIELDDEFINITIONSCONTROLLER_H
