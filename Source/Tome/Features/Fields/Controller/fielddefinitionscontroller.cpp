#include "fielddefinitionscontroller.h"

#include <stdexcept>

#include "../../../Util/listutils.h"
#include "../../../Util/stringutils.h"

using namespace Tome;


FieldDefinitionsController::FieldDefinitionsController()
{
}

const FieldDefinition FieldDefinitionsController::addFieldDefinition(const QString& id,
        const QString& displayName,
        const QString& fieldType,
        const QVariant& defaultValue,
        const QString& component,
        const QString& description,
        const QString& fieldDefinitionSetName)
{
    qInfo(QString("Adding field definition %1.").arg(id).toUtf8().constData());

    // Check if already exists.
    if (this->hasFieldDefinition(id))
    {
        const QString errorMessage = "Field with the specified id already exists: " + id;
        qCritical(errorMessage.toUtf8().constData());
        throw std::out_of_range(errorMessage.toStdString());
    }

    // Add new field.
    FieldDefinition fieldDefinition = FieldDefinition();
    fieldDefinition.id = stripWhitespaces(id);
    fieldDefinition.displayName = displayName;
    fieldDefinition.fieldType = fieldType;
    fieldDefinition.defaultValue = defaultValue;
    fieldDefinition.component = component;
    fieldDefinition.description = description;
    fieldDefinition.fieldDefinitionSetName = fieldDefinitionSetName;

    for (FieldDefinitionSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        FieldDefinitionSet& fieldDefinitionSet = *it;

        if (fieldDefinitionSet.name == fieldDefinitionSetName)
        {
            FieldDefinitionList& fieldDefinitions = fieldDefinitionSet.fieldDefinitions;
            int index = findInsertionIndex(fieldDefinitions, fieldDefinition, fieldDefinitionLessThanDisplayName);
            fieldDefinitions.insert(index, fieldDefinition);
            emit this->fieldDefinitionAdded(fieldDefinition);

            return fieldDefinition;
        }
    }

    const QString errorMessage = "Field definition set not found: " + fieldDefinitionSetName;
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
}

void FieldDefinitionsController::addFieldDefinitionSet(const FieldDefinitionSet& fieldDefinitionSet)
{
    // Update model.
    this->model->push_back(fieldDefinitionSet);
}

const FieldDefinition& FieldDefinitionsController::getFieldDefinition(const QString& id) const
{
    return *this->getFieldDefinitionById(id);
}

const FieldDefinitionList FieldDefinitionsController::getFieldDefinitions() const
{
    FieldDefinitionList fieldDefinitions;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = this->model->at(i);

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            fieldDefinitions << fieldDefinitionSet.fieldDefinitions.at(j);
        }
    }

    return fieldDefinitions;
}

const FieldDefinitionList FieldDefinitionsController::getFieldDefinitionsOfComponent(const QString& component) const
{
    FieldDefinitionList fieldDefinitions;

    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.count(); ++j)
        {
            FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.component == component)
            {
                fieldDefinitions << fieldDefinition;
            }
        }
    }

    return fieldDefinitions;
}

const FieldDefinitionSetList& FieldDefinitionsController::getFieldDefinitionSets() const
{
    return *this->model;
}

const QStringList FieldDefinitionsController::getFieldDefinitionSetNames() const
{
    QStringList names;

    for (int i = 0; i < this->model->count(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = this->model->at(i);
        names << fieldDefinitionSet.name;
    }

    return names;
}

bool FieldDefinitionsController::hasFieldDefinition(const QString& id) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.id == id)
            {
                return true;
            }
        }
    }

    return false;
}

int FieldDefinitionsController::indexOf(const FieldDefinition& fieldDefinition) const
{
    return this->model->at(0).fieldDefinitions.indexOf(fieldDefinition);
}

void FieldDefinitionsController::moveFieldDefinitionToSet(const QString& fieldDefinitionId, const QString& fieldDefinitionSetName)
{
    qInfo(QString("Moving field definition %1 to set %2.").arg(fieldDefinitionId, fieldDefinitionSetName)
          .toUtf8().constData());

    FieldDefinition fieldDefinition = this->getFieldDefinition(fieldDefinitionId);

    for (FieldDefinitionSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        FieldDefinitionSet& fieldDefinitionSet = *itSets;
        FieldDefinitionList& fieldDefinitions = fieldDefinitionSet.fieldDefinitions;

        // Check if should add field definition.
        if (fieldDefinitionSet.name == fieldDefinitionSetName)
        {
            int index = findInsertionIndex(fieldDefinitions, fieldDefinition, fieldDefinitionLessThanDisplayName);
            fieldDefinition.fieldDefinitionSetName = fieldDefinitionSetName;
            fieldDefinitions.insert(index, fieldDefinition);
            continue;
        }
        else
        {
            // Check if should remove field definition.
            for (FieldDefinitionList::iterator it = fieldDefinitions.begin();
                 it != fieldDefinitions.end();
                 ++it)
            {
                if ((*it).id == fieldDefinitionId)
                {
                    fieldDefinitions.erase(it);
                    break;
                }
            }
        }
    }
}

void FieldDefinitionsController::removeFieldComponent(const QString componentName)
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.count(); ++j)
        {
            FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.component == componentName)
            {
                fieldDefinition.component = QString();
            }
        }
    }
}

void FieldDefinitionsController::removeFieldDefinition(const QString& fieldId)
{
    qInfo(QString("Removing field definition %1.").arg(fieldId).toUtf8().constData());

    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (FieldDefinitionList::iterator it = fieldDefinitionSet.fieldDefinitions.begin();
             it != fieldDefinitionSet.fieldDefinitions.end();
             ++it)
        {
            if (it->id == fieldId)
            {
                emit this->fieldDefinitionRemoved(*it);
                fieldDefinitionSet.fieldDefinitions.erase(it);
                return;
            }
        }
    }
}

void FieldDefinitionsController::removeFieldDefinitionSet(const QString& name)
{
    for (FieldDefinitionSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if ((*it).name == name)
        {
            // Update model.
            this->model->erase(it);
            return;
        }
    }
}

void FieldDefinitionsController::renameFieldType(const QString oldTypeName, const QString newTypeName)
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        FieldDefinitionSet& fieldDefinitionSet = (*this->model)[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            if (fieldDefinition.fieldType == oldTypeName)
            {
                fieldDefinition.fieldType = newTypeName;
            }
        }
    }
}

void FieldDefinitionsController::setFieldDefinitionSets(FieldDefinitionSetList& model)
{
    this->model = &model;
}

void FieldDefinitionsController::updateFieldDefinition(const QString oldId,
        const QString newId,
        const QString& displayName,
        const QString& fieldType,
        const QVariant& defaultValue,
        const QString& component,
        const QString& description,
        const QString& fieldDefinitionSetName)
{
    // Check if already exists.
    if (oldId != newId && this->hasFieldDefinition(newId))
    {
        const QString errorMessage = "Field with the specified id already exists: " + newId;
        qCritical(errorMessage.toUtf8().constData());
        throw std::out_of_range(errorMessage.toStdString());
    }

    FieldDefinition& fieldDefinition = *this->getFieldDefinitionById(oldId);

    bool needsSorting = fieldDefinition.displayName != displayName;

    // Update model.
    fieldDefinition.id = stripWhitespaces(newId);
    fieldDefinition.displayName = displayName;
    fieldDefinition.fieldType = fieldType;
    fieldDefinition.defaultValue = defaultValue;
    fieldDefinition.description = description;
    fieldDefinition.component = component;

    // Move field definition, if necessary.
    if (fieldDefinition.fieldDefinitionSetName != fieldDefinitionSetName)
    {
        this->moveFieldDefinitionToSet(fieldDefinition.id, fieldDefinitionSetName);
    }

    // Sort by display name.
    if (needsSorting)
    {
        for (FieldDefinitionSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).fieldDefinitions.begin(), (*it).fieldDefinitions.end(), fieldDefinitionLessThanDisplayName);
        }
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

    const QString errorMessage = "Field not found: " + id;
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
}
