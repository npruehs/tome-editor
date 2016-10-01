#include "typescontroller.h"

#include <stdexcept>

#include "../Model/builtintype.h"
#include "../Model/vector.h"
#include "../../../Util/listutils.h"


using namespace Tome;


TypesController::TypesController()
{
}

void TypesController::addCustomTypeSet(const CustomTypeSet& customTypeSet)
{
    this->model->push_back(customTypeSet);
}

const CustomType TypesController::addEnumeration(const QString& name, const QStringList& enumeration)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.setEnumeration(enumeration);

    CustomTypeList& types = (*this->model)[0].types;
    int index = findInsertionIndex(types, newType, customTypeLessThanName);
    types.insert(index, newType);

    return newType;
}

const CustomType TypesController::addList(const QString& name, const QString& itemType)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.setItemType(itemType);

    CustomTypeList& types = (*this->model)[0].types;
    int index = findInsertionIndex(types, newType, customTypeLessThanName);
    types.insert(index, newType);

    return newType;
}

const CustomType TypesController::addMap(const QString& name, const QString& keyType, const QString& valueType)
{
    CustomType newType = CustomType();
    newType.name = name;
    newType.setKeyType(keyType);
    newType.setValueType(valueType);

    CustomTypeList& types = (*this->model)[0].types;
    int index = findInsertionIndex(types, newType, customTypeLessThanName);
    types.insert(index, newType);

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
    typeNames.push_back(BuiltInType::Vector2I);
    typeNames.push_back(BuiltInType::Vector2R);
    typeNames.push_back(BuiltInType::Vector3I);
    typeNames.push_back(BuiltInType::Vector3R);
    return typeNames;
}

const CustomType& TypesController::getCustomType(const QString& name) const
{
    return *this->getCustomTypeByName(name);
}

const CustomTypeList TypesController::getCustomTypes() const
{
    CustomTypeList types;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const CustomTypeSet& typeSet = this->model->at(i);

        for (int j = 0; j < typeSet.types.size(); ++j)
        {
            types << typeSet.types[j];
        }
    }

    return types;
}

const CustomTypeSetList& TypesController::getCustomTypeSets() const
{
    return *this->model;
}

const QStringList TypesController::getTypeNames() const
{
    QStringList typeNames = this->getBuiltInTypes();
    CustomTypeList types = this->getCustomTypes();

    for (int i = 0; i < types.size(); ++i)
    {
        const CustomType& type = types[i];
        typeNames.push_back(type.name);
    }

    return typeNames;
}

int TypesController::indexOf(const CustomType& customType) const
{
    return this->model->at(0).types.indexOf(customType);
}

bool TypesController::isBuiltInType(const QString& name) const
{
    return this->getBuiltInTypes().contains(name);
}

bool TypesController::isCustomType(const QString& name) const
{
    CustomTypeList types = this->getCustomTypes();

    for (int i = 0; i < types.size(); ++i)
    {
        const CustomType& type = types[i];
        if (type.name == name)
        {
            return true;
        }
    }

    return false;
}

void TypesController::removeCustomType(const QString& typeName)
{
    CustomTypeList& tpyes = (*this->model)[0].types;

    for (CustomTypeList::iterator it = tpyes.begin();
         it != tpyes.end();
         ++it)
    {
        if (it->name == typeName)
        {
            tpyes.erase(it);
            return;
        }
    }
}

void TypesController::removeCustomTypeAt(const int index)
{
    this->model->removeAt(index);
}

void TypesController::removeCustomTypeSet(const QString& name)
{
    for (CustomTypeSetList::iterator it = this->model->begin();
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

void TypesController::renameType(const QString oldName, const QString newName)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    // Rename type.
    type.name = newName;

    // Update list item type and map key and value type references.

    for (int i = 0; i < this->model->size(); ++i)
    {
        CustomTypeSet& typeSet = (*this->model)[i];

        for (int j = 0; j < typeSet.types.size(); ++j)
        {
            CustomType& t = typeSet.types[j];

            if (t.isList() && t.getItemType() == oldName)
            {
                t.setItemType(newName);
            }

            if (t.isMap())
            {
                if (t.getKeyType() == oldName)
                {
                    t.setKeyType(newName);
                }

                if (t.getValueType() == oldName)
                {
                    t.setValueType(newName);
                }
            }
        }
    }
}

void TypesController::setCustomTypes(CustomTypeSetList& model)
{
    this->model = &model;
}

void TypesController::updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setEnumeration(enumeration);

    if (needsSorting)
    {
        std::sort((*this->model)[0].types.begin(), (*this->model)[0].types.end(), customTypeLessThanName);
    }
}

void TypesController::updateList(const QString& oldName, const QString& newName, const QString& itemType)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setItemType(itemType);

    if (needsSorting)
    {
        std::sort((*this->model)[0].types.begin(), (*this->model)[0].types.end(), customTypeLessThanName);
    }
}

void TypesController::updateMap(const QString& oldName, const QString& newName, const QString& keyType, const QString& valueType)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setKeyType(keyType);
    type.setValueType(valueType);

    if (needsSorting)
    {
        std::sort((*this->model)[0].types.begin(), (*this->model)[0].types.end(), customTypeLessThanName);
    }
}

QString TypesController::valueToString(const QVariant& value, const QString& typeName)
{
    // Vector2I.
    if (typeName == BuiltInType::Vector2I || typeName == BuiltInType::Vector2R ||
        typeName == BuiltInType::Vector3I || typeName == BuiltInType::Vector3R)
    {
        QVariantMap map = value.toMap();

        QVariant x = map[BuiltInType::Vector::X];
        QVariant y = map[BuiltInType::Vector::Y];

        QString string = "(";
        string += x.toString();
        string += ", ";
        string += y.toString();

        if (typeName == BuiltInType::Vector3I || typeName == BuiltInType::Vector3R)
        {
            QVariant z = map[BuiltInType::Vector::Z];

            string += ", ";
            string += z.toString();
        }

        string += ")";

        return string;
    }

    // Custom list.
    if (this->isCustomType(typeName))
    {
        const CustomType& customType = this->getCustomType(typeName);

        if (customType.isList())
        {
            return toString(value.toList());
        }

        if (customType.isMap())
        {
            return toString(value.toMap());
        }
    }

    // Default.
    return value.toString();
}

CustomType* TypesController::getCustomTypeByName(const QString& name) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        CustomTypeSet& typeSet = (*this->model)[i];

        for (int j = 0; j < typeSet.types.size(); ++j)
        {
            CustomType& type = typeSet.types[j];

            if (type.name == name)
            {
                return &type;
            }
        }
    }

    const QString errorMessage = "Type not found: " + name;
    throw std::out_of_range(errorMessage.toStdString());
}
