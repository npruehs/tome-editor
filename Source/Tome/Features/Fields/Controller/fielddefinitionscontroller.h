#ifndef FIELDDEFINITIONSCONTROLLER_H
#define FIELDDEFINITIONSCONTROLLER_H

#include "../Model/fielddefinitionsetlist.h"
#include "../../Components/Model/component.h"

namespace Tome
{
    class ComponentsController;
    class TypesController;

    /**
     * @brief Controller for adding, updating and removing field definitions.
     */
    class FieldDefinitionsController : public QObject
    {
            Q_OBJECT

        public:
        /**
             * @brief Constructs a new controller for adding, updating and removing field definitions.
             * @param componentsController Controller for adding and removing components.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            FieldDefinitionsController(const ComponentsController& componentsController, const TypesController& typesController);

            /**
             * @brief Adds a new field definition to the specified set.
             *
             * @exception std::out_of_range if a field with the specified id already exists.
             * @exception std::out_of_range if the field definition set could not be found.
             *
             * @see hasFieldDefinition for checking whether a field definition with the specified id already exists.
             *
             * @param id Id of the field to add.
             * @param displayName Display name of the field to add.
             * @param fieldType Type of the field to add.
             * @param defaultValue Default value of the field to add.
             * @param component Component to add the field to.
             * @param description Textual description of the field to add.
             * @param fieldDefinitionSetName Name of the field definition set to add the field to.
             */
            void addFieldDefinition(
                    const QString& id,
                    const QString& displayName,
                    const QString& fieldType,
                    const QVariant& defaultValue,
                    const QString& component,
                    const QString& description,
                    const QString& fieldDefinitionSetName);

            /**
             * @brief Adds the passed field definition set to the project.
             * @param fieldDefinitionSet Field definition set to add.
             */
            void addFieldDefinitionSet(const FieldDefinitionSet& fieldDefinitionSet);

            /**
             * @brief Gets the field definition with the specified id.
             *
             * @exception std::out_of_range if the field definition with the specified id could not be found.
             *
             * @see hasFieldDefinition for checking whether a field definition with the specified id exists.
             *
             * @param id Id of the field definition to get.
             * @return Field definition with the specified id.
             */
            const FieldDefinition& getFieldDefinition(const QString& id) const;

            /**
             * @brief Gets a list of all field definitions in the project.
             * @return List of all field definitions in the project.
             */
            const FieldDefinitionList getFieldDefinitions() const;

            /**
             * @brief Gets a list of all field definitions belonging to the specified component.
             * @param component Component to get all fields of.
             * @return List of all field definitions belonging to the specified component.
             */
            const FieldDefinitionList getFieldDefinitionsOfComponent(const QString& component) const;

            /**
             * @brief Gets a list of all field definition sets in the project.
             * @return List of all field definition sets in the project.
             */
            const FieldDefinitionSetList& getFieldDefinitionSets() const;

            /**
             * @brief Gets a list of the names of all field definition sets in the project.
             * @return List of the names of all field definition sets in the project.
             */
            const QStringList getFieldDefinitionSetNames() const;

            /**
             * @brief Checks whether the project has a field definition with the specified id.
             * @param id Id of the field definition to look for.
             * @return true, if a field definition with the specified id exists, and false otherwise.
             */
            bool hasFieldDefinition(const QString& id) const;

            /**
             * @brief Removes the field definition with the specified id from the project.
             * @param fieldId Id of the field to remove.
             */
            void removeFieldDefinition(const QString& fieldId);

            /**
             * @brief Removes the field definition set with the specified name from the project, including all fields.
             * @param name Name of the field definition set to remove.
             */
            void removeFieldDefinitionSet(const QString& name);

            /**
             * @brief Uses the specified list of field definition sets as model for this controller.
             * @param model New list of field definition sets to use as model for this controller.
             */
            void setFieldDefinitionSets(FieldDefinitionSetList& model);

            /**
             * @brief Updates the properties of the specified field definition.
             *
             * @exception std::out_of_range if the field definition with the specified id could not be found.
             * @exception std::out_of_range if a field with the new id already exists.
             *
             * @see hasFieldDefinition for checking whether a field definition with the specified id exists.
             *
             * @param oldId Current id of the field to update.
             * @param newId New id of the field to update.
             * @param displayName New display name of the field.
             * @param fieldType New type of the field.
             * @param defaultValue New default value of the field.
             * @param component New component the field should belong to.
             * @param description New textual description of the field.
             * @param fieldDefinitionSetName Name of the new field definition set the field should belong to.
             */
            void updateFieldDefinition(const QString oldId,
                    const QString newId,
                    const QString& displayName,
                    const QString& fieldType,
                    const QVariant& defaultValue,
                    const QString& component,
                    const QString& description,
                    const QString& fieldDefinitionSetName);

        signals:
            /**
             * @brief A new field definition has been added to the project.
             * @param fieldDefinition New field defintion that has been added to the project.
             */
            void fieldDefinitionAdded(const Tome::FieldDefinition& fieldDefinition);

            /**
             * @brief A field definition has been removed from the project.
             * @param fieldDefinition Field definition that has been removed from the project.
             */
            void fieldDefinitionRemoved(const Tome::FieldDefinition& fieldDefinition);

            /**
             * @brief The properties of a field definition have been updated.
             * @param oldFieldDefinition Old properties of the field definition.
             * @param newFieldDefinition New properties of the field definition.
             */
            void fieldDefinitionUpdated(const Tome::FieldDefinition& oldFieldDefinition, const Tome::FieldDefinition& newFieldDefinition);

        private slots:
            void onComponentRemoved(const Tome::Component& component);
            void onTypeRenamed(const QString& oldName, const QString& newName);

        private:
            const ComponentsController& componentsController;
            const TypesController& typesController;

            FieldDefinitionSetList* model;

            FieldDefinition* getFieldDefinitionById(const QString& id) const;
            void moveFieldDefinitionToSet(const QString& fieldDefinitionId, const QString& fieldDefinitionSetName);
    };
}

#endif // FIELDDEFINITIONSCONTROLLER_H
