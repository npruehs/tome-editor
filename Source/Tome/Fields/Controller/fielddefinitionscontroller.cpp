#include "fielddefinitionscontroller.h"

#include <stdexcept>

using namespace Tome;


FieldDefinitionsController::FieldDefinitionsController()
{
}

const FieldDefinition& FieldDefinitionsController::getFieldDefinition(const QString& id) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = this->model->at(i);

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            const FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.id == id)
            {
                return fieldDefinition;
            }
        }
    }

    throw std::out_of_range(id.toStdString());
}

const FieldDefinitionSetList&FieldDefinitionsController::getFieldDefinitionSets() const
{
    return *this->model;
}

void FieldDefinitionsController::setFieldDefinitionSets(FieldDefinitionSetList& model)
{
    this->model = &model;
}
