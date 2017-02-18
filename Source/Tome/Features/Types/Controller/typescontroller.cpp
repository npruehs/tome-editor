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

const CustomType TypesController::addDerivedType(const QString& name, const QString& baseType, const QVariantMap& facets, const QString& customTypeSetName)
{
    qInfo(QString("Adding derived type %1 with base type %2.").arg(name, baseType).toUtf8().constData());

    CustomType newType = CustomType();
    newType.name = name;
    newType.constrainingFacets = facets;
    newType.setBaseType(baseType);

    this->addCustomType(newType, customTypeSetName);

    return newType;
}

const CustomType TypesController::addEnumeration(const QString& name, const QStringList& enumeration, const QString& customTypeSetName)
{
    qInfo(QString("Adding enumeration %1.").arg(name).toUtf8().constData());

    CustomType newType = CustomType();
    newType.name = name;
    newType.setEnumeration(enumeration);

    this->addCustomType(newType, customTypeSetName);

    return newType;
}

const CustomType TypesController::addList(const QString& name, const QString& itemType, const QString& customTypeSetName)
{
    qInfo(QString("Adding list %1.").arg(name).toUtf8().constData());

    CustomType newType = CustomType();
    newType.name = name;
    newType.setItemType(itemType);

    this->addCustomType(newType, customTypeSetName);

    return newType;
}

const CustomType TypesController::addMap(const QString& name, const QString& keyType, const QString& valueType, const QString& customTypeSetName)
{
    qInfo(QString("Adding map %1.").arg(name).toUtf8().constData());

    CustomType newType = CustomType();
    newType.name = name;
    newType.setKeyType(keyType);
    newType.setValueType(valueType);

    this->addCustomType(newType, customTypeSetName);

    return newType;
}

const QStringList TypesController::getBuiltInTypes() const
{
    QStringList typeNames;
    typeNames.push_back(BuiltInType::Boolean);
    typeNames.push_back(BuiltInType::Color);
    typeNames.push_back(BuiltInType::File);
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

const QStringList TypesController::getCustomTypeSetNames() const
{
    QStringList names;

    for (int i = 0; i < this->model->count(); ++i)
    {
        const CustomTypeSet& customTypeSet = this->model->at(i);
        names << customTypeSet.name;
    }

    return names;
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

    typeNames.sort();
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

bool TypesController::isTypeOrDerivedFromType(const QString& lhs, const QString& rhs) const
{
    if (lhs == rhs)
    {
        return true;
    }

    if (!this->isCustomType(lhs))
    {
        return false;
    }

    const CustomType& type = this->getCustomType(lhs);
    return type.isDerivedType() && type.getBaseType() == rhs;
}

void TypesController::removeCustomType(const QString& typeName)
{
    qInfo(QString("Removing custom type %1.").arg(typeName).toUtf8().constData());

    for (CustomTypeSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        CustomTypeList& types = (*itSets).types;

        for (CustomTypeList::iterator it = types.begin();
             it != types.end();
             ++it)
        {
            if (it->name == typeName)
            {
                emit this->typeRemoved(*it);
                types.erase(it);
                return;
            }
        }
    }
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

void TypesController::setCustomTypes(CustomTypeSetList& model)
{
    this->model = &model;
}

void TypesController::updateDerivedType(const QString& oldName, const QString& newName, const QString& baseType, const QVariantMap facets, const QString& typeSetName)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setBaseType(baseType);
    type.constrainingFacets = facets;

    // Move type to other set, if necessary.
    if (type.typeSetName != typeSetName)
    {
        this->moveCustomTypeToSet(type.name, typeSetName);
    }

    if (needsSorting)
    {
        for (CustomTypeSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).types.begin(), (*it).types.end(), customTypeLessThanName);
        }
    }

    // Notify listeners.
    emit this->typeUpdated(type);
}

void TypesController::updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration, const QString& typeSetName)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setEnumeration(enumeration);

    // Move type to other set, if necessary.
    if (type.typeSetName != typeSetName)
    {
        this->moveCustomTypeToSet(type.name, typeSetName);
    }

    if (needsSorting)
    {
        for (CustomTypeSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).types.begin(), (*it).types.end(), customTypeLessThanName);
        }
    }

    // Notify listeners.
    emit this->typeUpdated(type);
}

void TypesController::updateList(const QString& oldName, const QString& newName, const QString& itemType, const QString& typeSetName)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setItemType(itemType);

    // Move type to other set, if necessary.
    if (type.typeSetName != typeSetName)
    {
        this->moveCustomTypeToSet(type.name, typeSetName);
    }

    if (needsSorting)
    {
        for (CustomTypeSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).types.begin(), (*it).types.end(), customTypeLessThanName);
        }
    }

    // Notify listeners.
    emit this->typeUpdated(type);
}

void TypesController::updateMap(const QString& oldName, const QString& newName, const QString& keyType, const QString& valueType, const QString& typeSetName)
{
    CustomType& type = *this->getCustomTypeByName(oldName);

    bool needsSorting = type.name != newName;

    this->renameType(oldName, newName);
    type.setKeyType(keyType);
    type.setValueType(valueType);

    // Move type to other set, if necessary.
    if (type.typeSetName != typeSetName)
    {
        this->moveCustomTypeToSet(type.name, typeSetName);
    }

    if (needsSorting)
    {
        for (CustomTypeSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).types.begin(), (*it).types.end(), customTypeLessThanName);
        }
    }

    // Notify listeners.
    emit this->typeUpdated(type);
}

QString TypesController::valueToString(const QVariant& value, const QString& typeName) const
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

void TypesController::addCustomType(CustomType customType, const QString& customTypeSetName)
{
    customType.typeSetName = customTypeSetName;

    for (CustomTypeSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        CustomTypeSet& customTypeSet = *it;

        if (customTypeSet.name == customTypeSetName)
        {
            CustomTypeList& types = customTypeSet.types;
            int index = findInsertionIndex(types, customType, customTypeLessThanName);
            types.insert(index, customType);
            emit this->typeAdded(customType);
            return;
        }
    }

    const QString errorMessage = "Custom type set not found: " + customTypeSetName;
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
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
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
}

void TypesController::moveCustomTypeToSet(const QString& customTypeName, const QString& customTypeSetName)
{
    qInfo(QString("Moving type %1 to set %2.").arg(customTypeName, customTypeSetName).toUtf8().constData());

    CustomType customType = this->getCustomType(customTypeName);

    for (CustomTypeSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        CustomTypeSet& customTypeSet = *itSets;
        CustomTypeList& customTypes = customTypeSet.types;

        // Check if should add custom type.
        if (customTypeSet.name == customTypeSetName)
        {
            int index = findInsertionIndex(customTypes, customType, customTypeLessThanName);
            customType.typeSetName = customTypeSetName;
            customTypes.insert(index, customType);
            continue;
        }
        else
        {
            // Check if should remove custom type.
            for (CustomTypeList::iterator it = customTypes.begin();
                 it != customTypes.end();
                 ++it)
            {
                if ((*it).name == customTypeName)
                {
                    customTypes.erase(it);
                    break;
                }
            }
        }
    }
}

void TypesController::renameType(const QString oldName, const QString newName)
{
    if (oldName == newName)
    {
        return;
    }

    qInfo(QString("Renaming custom type %1 to %2.").arg(oldName, newName).toUtf8().constData());

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

    // Notify listeners.
    emit this->typeRenamed(oldName, newName);
}
