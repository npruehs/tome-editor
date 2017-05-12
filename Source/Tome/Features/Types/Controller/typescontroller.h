#ifndef TYPESCONTROLLER_H
#define TYPESCONTROLLER_H

#include <QVariant>

#include "../Model/customtypelist.h"
#include "../Model/customtypesetlist.h"

namespace Tome
{
    /**
     * @brief Controller for adding, updating and removing custom types.
     */
    class TypesController : public QObject
    {
            Q_OBJECT

        public:
            TypesController();

            void addCustomTypeSet(const CustomTypeSet& customTypeSet);

            const CustomType addDerivedType(const QString& name, const QString& baseType, const QVariantMap& facets, const QString& customTypeSetName);
            const CustomType addEnumeration(const QString& name, const QStringList& enumeration, const QString& customTypeSetName);
            const CustomType addList(const QString& name, const QString& itemType, const QString& customTypeSetName);
            const CustomType addMap(const QString& name, const QString& keyType, const QString& valueType, const QString& customTypeSetName);

            const QStringList getBuiltInTypes() const;

            /**
             * @brief getCustomType Returns the custom type with the specified name.
             * @param name Name of the type to get.
             * @return Custom type with the specified name.
             */
            const CustomType& getCustomType(const QString& name) const;

            const CustomTypeList getCustomTypes() const;
            const CustomTypeSetList& getCustomTypeSets() const;
            const QStringList getCustomTypeSetNames() const;

            /**
             * @brief getCustomTypeNames Returns a list of all type names of this project, including built-in types.
             * @return List containing the names of all types avaialable in this project.
             */
            const QStringList getTypeNames() const;

            int indexOf(const CustomType& customType) const;
            bool isBuiltInType(const QString& name) const;
            bool isCustomType(const QString& name) const;
            bool isTypeOrDerivedFromType(const QString& lhs, const QString& rhs) const;
            void removeCustomType(const QString& typeName);
            void removeCustomTypeSet(const QString& name);
            void setCustomTypes(CustomTypeSetList& model);

            void updateDerivedType(const QString& oldName, const QString& newName, const QString& baseType, const QVariantMap facets, const QString& typeSetName);
            void updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration, const QString& typeSetName);
            void updateList(const QString& oldName, const QString& newName, const QString& itemType, const QString& typeSetName);
            void updateMap(const QString& oldName, const QString& newName, const QString& keyType, const QString& valueType, const QString& typeSetName);

            QString valueToString(const QVariant& value, const QString& typeName) const;

        signals:
            void typeAdded(const Tome::CustomType& type);
            void typeRenamed(const QString& oldName, const QString& newName);
            void typeRemoved(const Tome::CustomType& type);
            void typeUpdated(const Tome::CustomType& type);

        private:
            CustomTypeSetList* model;

            void addCustomType(CustomType customType, const QString& customTypeSetName);
            CustomType* getCustomTypeByName(const QString& name) const;
            void moveCustomTypeToSet(const QString& customTypeName, const QString& customTypeSetName);
            void renameType(const QString oldName, const QString newName);
    };
}

#endif // TYPESCONTROLLER_H
