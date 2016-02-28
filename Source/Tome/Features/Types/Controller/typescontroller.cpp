#include "typescontroller.h"

#include <stdexcept>

#include "../Model/builtintype.h"
#include "../../../Util/listutils.h"


using namespace Tome;


TypesController::TypesController()
{
}

const CustomType TypesController::addEnumeration(const QString& name, const QStringList& enumeration)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.setEnumeration(enumeration);

    int index = findInsertionIndex(*this->model, newType, customTypeLessThanName);
    this->model->insert(index, newType);

    return newType;
}

const CustomType TypesController::addList(const QString& name, const QString& itemType)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.setItemType(itemType);

    int index = findInsertionIndex(*this->model, newType, customTypeLessThanName);
    this->model->insert(index, newType);

    return newType;
}

const QStringList TypesController::getBuiltInTypes() const
{
    QStringList typeNames;
    typeNames.push_back(BuiltInType::Boolean);
    typeNames.push_back(BuiltInType::Color);
    typeNames.push_back(BuiltInType::Integer);
    typeNames.push_back(BuiltInType::Real);
    typeNames.push_back(BuiltInType::Reference);
    typeNames.push_back(BuiltInType::String);
    return typeNames;
}

const CustomType& TypesController::getCustomType(const QString& name) const
{
    return *this->getCustomTypeByName(name);
}

const CustomTypeList& TypesController::getCustomTypes() const
{
    return *this->model;
}

const QStringList TypesController::getTypeNames() const
{
    QStringList typeNames = this->getBuiltInTypes();

    for (int i = 0; i < this->model->size(); ++i)
    {
        const CustomType& type = this->model->at(i);
        typeNames.push_back(type.name);
    }

    return typeNames;
}

int TypesController::indexOf(const CustomType& customType) const
{
    return this->model->indexOf(customType);
}

bool TypesController::isBuiltInType(const QString& name) const
{
    return this->getBuiltInTypes().contains(name);
}

bool TypesController::isCustomType(const QString& name) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        const CustomType& type = this->model->at(i);
        if (type.name == name)
        {
            return true;
        }
    }

    return false;
}

void TypesController::removeCustomTypeAt(const int index)
{
    this->model->removeAt(index);
}

void TypesController::setCustomTypes(CustomTypeList& model)
{
    this->model = &model;
}

void TypesController::updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    type.name = newName;
    type.setEnumeration(enumeration);

    if (needsSorting)
    {
        std::sort(this->model->begin(), this->model->end(), customTypeLessThanName);
    }
}

void TypesController::updateList(const QString& oldName, const QString& newName, const QString& itemType)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    type.name = newName;
    type.setItemType(itemType);

    if (needsSorting)
    {
        std::sort(this->model->begin(), this->model->end(), customTypeLessThanName);
    }
}

CustomType* TypesController::getCustomTypeByName(const QString& name) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        CustomType& type = (*this->model)[i];
        if (type.name == name)
        {
            return &type;
        }
    }

    const QString errorMessage = "Type not found: " + name;
    throw std::out_of_range(errorMessage.toStdString());
}
