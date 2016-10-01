#ifndef TYPESCONTROLLER_H
#define TYPESCONTROLLER_H

#include <QVariant>

#include "../Model/customtypelist.h"
#include "../Model/customtypesetlist.h"

namespace Tome
{
    class TypesController
    {
        public:
            TypesController();

            void addCustomTypeSet(const CustomTypeSet& customTypeSet);
            const CustomType addEnumeration(const QString& name, const QStringList& enumeration);
            const CustomType addList(const QString& name, const QString& itemType);
            const CustomType addMap(const QString& name, const QString& keyType, const QString& valueType);

            const QStringList getBuiltInTypes() const;

            /**
             * @brief getCustomType Returns the custom type with the specified name.
             * @param name Name of the type to get.
             * @return Custom type with the specified name.
             */
            const CustomType& getCustomType(const QString& name) const;

            const CustomTypeList getCustomTypes() const;
            const CustomTypeSetList& getCustomTypeSets() const;

            /**
             * @brief getCustomTypeNames Returns a list of all type names of this project, including built-in types.
             * @return List containing the names of all types avaialable in this project.
             */
            const QStringList getTypeNames() const;

            int indexOf(const CustomType& customType) const;
            bool isBuiltInType(const QString& name) const;
            bool isCustomType(const QString& name) const;
            void removeCustomType(const QString& typeName);
            void removeCustomTypeAt(const int index);
            void removeCustomTypeSet(const QString& name);
            void renameType(const QString oldName, const QString newName);
            void setCustomTypes(CustomTypeSetList& model);
            void updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration);
            void updateList(const QString& oldName, const QString& newName, const QString& itemType);
            void updateMap(const QString& oldName, const QString& newName, const QString& keyType, const QString& valueType);

            QString valueToString(const QVariant& value, const QString& typeName);


        private:
            CustomTypeSetList* model;

            CustomType* getCustomTypeByName(const QString& name) const;
    };
}

#endif // TYPESCONTROLLER_H
