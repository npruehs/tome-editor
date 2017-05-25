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
            /**
             * @brief Adds the passed custom type set to the project.
             * @param customTypeSet Custom type set to add.
             */
            void addCustomTypeSet(const CustomTypeSet& customTypeSet);

            /**
             * @brief Adds a new derived type to the specified set.
             *
             * @exception std::out_of_range if the custom type set could not be found.
             *
             * @param name Name of the type to add.
             * @param baseType Name of the base type of the type to add.
             * @param facets Facet values of the type to add.
             * @param customTypeSetName Name of the type set to add the new type to.
             * @return Copy of the added custom type.
             */
            const CustomType addDerivedType(const QString& name,
                                            const QString& baseType,
                                            const QVariantMap& facets,
                                            const QString& customTypeSetName);

            /**
             * @brief Adds a new enumeration type to the specified set.
             *
             * @exception std::out_of_range if the custom type set could not be found.
             *
             * @param name Name of the enumeration to add.
             * @param enumeration Items of the enumeration to add.
             * @param customTypeSetName Name of the type set to add the new enumeration to.
             * @return Copy of the added enumeration.
             */
            const CustomType addEnumeration(const QString& name,
                                            const QStringList& enumeration,
                                            const QString& customTypeSetName);

            /**
             * @brief Adds a new list type to the specified set.
             *
             * @exception std::out_of_range if the custom type set could not be found.
             *
             * @param name Name of the list type to add.
             * @param itemType Type of the items of the list type to add.
             * @param customTypeSetName Name of the type set to add the new list type to.
             * @return Copy of the added list type.
             */
            const CustomType addList(const QString& name,
                                     const QString& itemType,
                                     const QString& customTypeSetName);

            /**
             * @brief Adds a new map type to the specified set.
             *
             * @exception std::out_of_range if the custom type set could not be found.
             *
             * @param name Name of the map type to add.
             * @param keyType Type of the keys of the map type to add.
             * @param valueType Type of the values of the map type to add.
             * @param customTypeSetName Name of the type set to add the new map type to.
             * @return Copy of the added map type.
             */
            const CustomType addMap(const QString& name,
                                    const QString& keyType,
                                    const QString& valueType,
                                    const QString& customTypeSetName);

            /**
             * @brief Gets a list of built-in (i.e. non-custom) types of Tome.
             * @return List of all built-in types of Tome.
             */
            const QStringList getBuiltInTypes() const;

            /**
             * @brief Gets the custom type with the specified name.
             *
             * @throws std::out_of_range if the custom type with the specified name could not be found.
             *
             * @see isCustomType for checking whether a custom type with the specified name exists.
             *
             * @param name Name of the type to get.
             * @return Custom type with the specified name.
             */
            const CustomType& getCustomType(const QString& name) const;

            /**
             * @brief Gets a list of all custom types in the project.
             * @return List of all custom types in the project.
             */
            const CustomTypeList getCustomTypes() const;

            /**
             * @brief Gets a list of all custom type sets in the project.
             * @return List of all custom type sets in the project.
             */
            const CustomTypeSetList& getCustomTypeSets() const;

            /**
             * @brief Gets a list of the names of all custom type sets in the project.
             * @return List of the names of all custom type sets in the project.
             */
            const QStringList getCustomTypeSetNames() const;

            /**
             * @brief Gets a list of all type names of this project, including built-in types.
             * @return List containing the names of all types avaialable in this project.
             */
            const QStringList getTypeNames() const;

            /**
             * @brief Checks whether the specified name denotes a built-in type of Tome.
             * @param name Name of the type to check.
             * @return true, if the specified name denotes a built-in type of Tome, and false otherwise.
             */
            bool isBuiltInType(const QString& name) const;

            /**
             * @brief Checks whether the project has a custom type with the specified name.
             * @param name Name of the custom type to look for.
             * @return true, if a custom type with the specified name exists, and false otherwise.
             */
            bool isCustomType(const QString& name) const;

            /**
             * @brief Checks whether the first specified type is equal to or derived from the second specified type.
             * @param lhs Name of the type to check.
             * @param rhs Possible equal or base type.
             * @return true, if the first specified type is equal to or derived from the second specified type, and false otherwise.
             */
            bool isTypeOrDerivedFromType(const QString& lhs, const QString& rhs) const;

            /**
             * @brief Removes the custom type with the specified name from the project.
             * @param typeName Name of the custom type to remove.
             */
            void removeCustomType(const QString& typeName);

            /**
             * @brief Removes the custom type set with the specified name from the project, including all custom types.
             * @param name Name of the custom type set to remove.
             */
            void removeCustomTypeSet(const QString& name);

            /**
             * @brief Uses the specified list of custom type sets as model for this controller.
             * @param model New list of custom type sets to use as model for this controller.
             */
            void setCustomTypes(CustomTypeSetList& model);

            /**
             * @brief Updates the properties of the specified derived type.
             *
             * @throws std::out_of_range if the custom type with the specified name could not be found.
             *
             * @see isCustomType for checking whether a custom type with the specified name exists.
             *
             * @param oldName Current name of the derived type to update.
             * @param newName New name of the derived type to update.
             * @param baseType Name of the new base type of the type to update.
             * @param facets New facet values of the type to update.
             * @param typeSetName Name of the new type set the derived type should belong to.
             */
            void updateDerivedType(const QString& oldName,
                                   const QString& newName,
                                   const QString& baseType,
                                   const QVariantMap facets,
                                   const QString& typeSetName);

            /**
             * @brief Updates the properties of the specified enumeration.
             *
             * @throws std::out_of_range if the custom type with the specified name could not be found.
             *
             * @see isCustomType for checking whether a custom type with the specified name exists.
             *
             * @param oldName Current name of the enumeration to update.
             * @param newName New name of the enumeration to update.
             * @param enumeration New items of the enumeration to update.
             * @param typeSetName Name of the new type set the enumeration should belong to.
             */
            void updateEnumeration(const QString& oldName,
                                   const QString& newName,
                                   const QStringList& enumeration,
                                   const QString& typeSetName);

            /**
             * @brief Updates the properties of the specified list type.
             *
             * @throws std::out_of_range if the custom type with the specified name could not be found.
             *
             * @see isCustomType for checking whether a custom type with the specified name exists.
             *
             * @param oldName Current name of the list type to update.
             * @param newName New name of the list type to update.
             * @param itemType New type of the items of the list type to update.
             * @param typeSetName Name of the new type set the list type should belong to.
             */
            void updateList(const QString& oldName,
                            const QString& newName,
                            const QString& itemType,
                            const QString& typeSetName);

            /**
             * @brief Updates the properties of the specified map type.
             *
             * @throws std::out_of_range if the custom type with the specified name could not be found.
             *
             * @see isCustomType for checking whether a custom type with the specified name exists.
             *
             * @param oldName Current name of the map type to update.
             * @param newName New name of the map type to update.
             * @param keyType New type of the keys of the map type to update.
             * @param valueType New type of the values of the map type to update.
             * @param typeSetName Name of the new type set the map type should belong to.
             */
            void updateMap(const QString& oldName,
                           const QString& newName,
                           const QString& keyType,
                           const QString& valueType,
                           const QString& typeSetName);

            /**
             * @brief Converts the specified value to a human-readable string.
             * @param value Value to convert.
             * @param typeName Name of the type of the value.
             * @return Human-readable string representation of the specified value.
             */
            QString valueToString(const QVariant& value, const QString& typeName) const;

        signals:
            /**
             * @brief A new custom type has been added to the project.
             * @param type New custom type.
             */
            void typeAdded(const Tome::CustomType& type);

            /**
             * @brief A custom type has ben renamed.
             * @param oldName Previous name of the custom type.
             * @param newName New name of the custom type.
             */
            void typeRenamed(const QString& oldName, const QString& newName);

            /**
             * @brief A custom type has been removed from the project.
             * @param type Custom type that has been removed from the project.
             */
            void typeRemoved(const Tome::CustomType& type);

            /**
             * @brief The properties of a custom type have been updated.
             * @param type Custom type that has been updated.
             */
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
