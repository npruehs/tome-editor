#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

#include <QMap>
#include <QString>
#include <QStringList>

namespace Tome
{
    class CustomType
    {
        public:
            CustomType();

            QString name;
            QMap<QString, QString> restrictions;
            QString typeSetName;

            /**
             * @brief getEnumeration Convenience function for retrieving the Enumeration restriction of this type, if available. Returns an empty list, if not.
             * @return Enumeration of allowed field values, if available, and an empty list otherwise.
             */
            QStringList getEnumeration() const;

            /**
             * @brief getItemType Convenience function for retrieving the Item Type restriction of this type, if available. Returns an empty string, if not.
             * @return Item Type of this list, if available, and an empty string otherwise.
             */
            QString getItemType() const;

            /**
             * @brief getKeyType Convenience function for retrieving the Key Type restriction of this type, if available. Returns an empty string, if not.
             * @return Key Type of this map, if available, and an empty string otherwise.
             */
            QString getKeyType() const;

            /**
             * @brief getKeyType Convenience function for retrieving the Value Type restriction of this type, if available. Returns an empty string, if not.
             * @return Value Type of this map, if available, and an empty string otherwise.
             */
            QString getValueType() const;


            bool isEnumeration() const;
            bool isList() const;
            bool isMap() const;

            /**
             * @brief setEnumeration Sets the Enumeration restriction of this type.
             * @param enumeration Allowed field values.
             */
            void setEnumeration(const QStringList& enumeration);

            /**
             * @brief setItemType Sets the Item Type restriction of this type.
             * @param itemType Type of the list items.
             */
            void setItemType(const QString& itemType);

            /**
             * @brief setItemType Sets the Key Type restriction of this type.
             * @param itemType Type of the map keys.
             */
            void setKeyType(const QString& keyType);

            /**
             * @brief setItemType Sets the Value Type restriction of this type.
             * @param itemType Type of the map values.
             */
            void setValueType(const QString& valueType);

        private:
            static const QString RestrictionEnumeration;
            static const QString RestrictionItemType;
            static const QString RestrictionKeyType;
            static const QString RestrictionValueType;
    };

    inline bool operator==(const CustomType& lhs, const CustomType& rhs){ return lhs.name == rhs.name; }
    inline bool operator!=(const CustomType& lhs, const CustomType& rhs){ return !(lhs == rhs); }

    inline bool customTypeLessThanName(const CustomType& e1, const CustomType& e2)
    {
        return e1.name.toLower() < e2.name.toLower();
    }
}

#endif // CUSTOMTYPE_H
