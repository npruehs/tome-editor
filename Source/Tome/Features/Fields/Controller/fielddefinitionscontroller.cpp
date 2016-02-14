#include "fielddefinitionscontroller.h"

#include <stdexcept>

#include "../../../Util/listutils.h"


using namespace Tome;


FieldDefinitionsController::FieldDefinitionsController()
{
}

const FieldDefinition FieldDefinitionsController::addFieldDefinition(const QString& id, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& component, const QString& description)
{
    FieldDefinition fieldDefinition = FieldDefinition();
    fieldDefinition.id = id;
    fieldDefinition.displayName = displayName;
    fieldDefinition.fieldType = fieldType;
    fieldDefinition.defaultValue = defaultValue;
    fieldDefinition.component = component;
    fieldDefinition.description = description;

    FieldDefinitionList& fieldDefinitions = (*this->model)[0].fieldDefinitions;
    int index = findInsertionIndex(fieldDefinitions, fieldDefinition, fieldDefinitionLessThanDisplayName);
    fieldDefinitions.insert(index, fieldDefinition);

    return fieldDefinition;
}

const FieldDefinition& FieldDefinitionsController::getFieldDefinition(const QString& id) const
{
    return *this->getFieldDefinitionById(id);
}

const FieldDefinitionSetList& FieldDefinitionsController::getFieldDefinitionSets() const
{
    return *this->model;
}

int FieldDefinitionsController::indexOf(const FieldDefinition& fieldDefinition) const
{
    return this->model->at(0).fieldDefinitions.indexOf(fieldDefinition);
}

void FieldDefinitionsController::removeFieldDefinitionAt(const int index)
{
    (*this->model)[0].fieldDefinitions.removeAt(index);
}

void FieldDefinitionsController::setFieldDefinitionSets(FieldDefinitionSetList& model)
{
    this->model = &model;
}

void FieldDefinitionsController::updateFieldDefinition(const QString& oldId, const QString& newId, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& component, const QString& description)
{
    FieldDefinition& fieldDefinition = *this->getFieldDefinitionById(oldId);

    bool needsSorting = fieldDefinition.displayName != displayName;

    // Update model.
    fieldDefinition.id = newId;
    fieldDefinition.displayName = displayName;
    fieldDefinition.fieldType = fieldType;
    fieldDefinition.defaultValue = defaultValue;
    fieldDefinition.description = description;
    fieldDefinition.component = component;

    // Sort by display name.
    if (needsSorting)
    {
        std::sort((*this->model)[0].fieldDefinitions.begin(), (*this->model)[0].fieldDefinitions.end(), fieldDefinitionLessThanDisplayName);
    }
}

FieldDefinition* FieldDefinitionsController::getFieldDefinitionById(const QString& id) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.id == id)
            {
                return &fieldDefinition;
            }
        }
    }

    throw std::out_of_range(id.toStdString());
}
