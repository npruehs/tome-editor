#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace Tome
{
    /**
     * @brief User-defined type of any field.
     */
    class CustomType
    {
        public:
            /**
             * @brief Name of this custom type.
             */
            QString name;

            /**
             * @brief Values of fundamental facets of this type, e.g. type of the keys of a map.
             */
            QVariantMap fundamentalFacets;

            /**
             * @brief Values of constraining facets of this type, e.g. maximum value of an integer.
             */
            QVariantMap constrainingFacets;

            /**
             * @brief Name of the set this custom type belongs to.
             */
            QString typeSetName;

            /**
             * @brief Convenience function for retrieving the BaseType fundamental facet of this type, if available. Returns an empty string, if not.
             * @return BaseType of this derived type, if available, and an empty string otherwise.
             */
            QString getBaseType() const;

            /**
             * @brief Convenience function for retrieving the Enumeration fundamental facet of this type, if available. Returns an empty list, if not.
             * @return Enumeration of allowed field values, if available, and an empty list otherwise.
             */
            QStringList getEnumeration() const;

            /**
             * @brief Convenience function for retrieving the Item Type fundamental facet of this type, if available. Returns an empty string, if not.
             * @return Item Type of this list, if available, and an empty string otherwise.
             */
            QString getItemType() const;

            /**
             * @brief Convenience function for retrieving the Key Type fundamental facet of this type, if available. Returns an empty string, if not.
             * @return Key Type of this map, if available, and an empty string otherwise.
             */
            QString getKeyType() const;

            /**
             * @brief Convenience function for retrieving the Value Type fundamental facet of this type, if available. Returns an empty string, if not.
             * @return Value Type of this map, if available, and an empty string otherwise.
             */
            QString getValueType() const;

            /**
             * @brief Checks whether this custom type is a derived type.
             * @return true, if this custom type is a derived type, and false otherwise.
             */
            bool isDerivedType() const;

            /**
             * @brief Checks whether this custom type is an enumeration.
             * @return true, if this custom type is an enumeration, and false otherwise.
             */
            bool isEnumeration() const;

            /**
             * @brief Checks whether this custom type is a list type.
             * @return true, if this custom type is a list type, and false otherwise.
             */
            bool isList() const;

            /**
             * @brief Checks whether this custom type is a map type.
             * @return true, if this custom type is a map type, and false otherwise.
             */
            bool isMap() const;

            /**
             * @brief Sets the BaseType fundamental facet of this type.
             * @param baseType Type this type is based on.
             */
            void setBaseType(const QString& baseType);

            /**
             * @brief Sets the Enumeration fundamental facet of this type.
             * @param enumeration Allowed values for fields of this type.
             */
            void setEnumeration(const QStringList& enumeration);

            /**
             * @brief Sets the Item Type fundamental facet of this type.
             * @param itemType Type of the list items.
             */
            void setItemType(const QString& itemType);

            /**
             * @brief Sets the Key Type fundamental facet of this type.
             * @param itemType Type of the map keys.
             */
            void setKeyType(const QString& keyType);

            /**
             * @brief Sets the Value Type fundamental facet of this type.
             * @param itemType Type of the map values.
             */
            void setValueType(const QString& valueType);

        private:
            static const QString FacetBaseType;
            static const QString FacetEnumeration;
            static const QString FacetItemType;
            static const QString FacetKeyType;
            static const QString FacetValueType;
    };

    inline bool operator==(const CustomType& lhs, const CustomType& rhs){ return lhs.name == rhs.name; }
    inline bool operator!=(const CustomType& lhs, const CustomType& rhs){ return !(lhs == rhs); }

    inline bool customTypeLessThanName(const CustomType& e1, const CustomType& e2)
    {
        return e1.name.toLower() < e2.name.toLower();
    }
}

#endif // CUSTOMTYPE_H
