#include "typescontroller.h"

#include <stdexcept>

#include "../builtintype.h"
#include "../../Util/vectorutils.h"


using namespace Tome;


bool lessThanCustomTypes(const CustomType& e1, const CustomType& e2)
{
    return e1.name.toLower() < e2.name.toLower();
}


TypesController::TypesController()
{
}

const CustomType TypesController::addEnumeration(const QString& name, const QStringList& enumeration)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.baseType = BuiltInType::String;
    newType.setEnumeration(enumeration);

    int index = findInsertionIndex(*this->model, newType, lessThanCustomTypes);
    this->model->insert(index, newType);

    return newType;
}

const CustomType TypesController::addList(const QString& name, const QString& itemType)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.baseType = BuiltInType::None;
    newType.setItemType(itemType);

    int index = findInsertionIndex(*this->model, newType, lessThanCustomTypes);
    this->model->insert(index, newType);

    return newType;
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
    QStringList typeNames;
    typeNames.push_back(BuiltInType::Boolean);
    typeNames.push_back(BuiltInType::Color);
    typeNames.push_back(BuiltInType::Integer);
    typeNames.push_back(BuiltInType::Real);
    typeNames.push_back(BuiltInType::Reference);
    typeNames.push_back(BuiltInType::String);

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
        std::sort(this->model->begin(), this->model->end(), lessThanCustomTypes);
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
        std::sort(this->model->begin(), this->model->end(), lessThanCustomTypes);
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

    throw std::out_of_range(name.toStdString());
}
