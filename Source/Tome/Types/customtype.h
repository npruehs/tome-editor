#ifndef CUSTOMFIELDTYPE_H
#define CUSTOMFIELDTYPE_H

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
            QString baseType;
            QMap<QString, QString> restrictions;

            /**
             * @brief getEnumeration Convenience function for retrieving the Enumeration restriction of this type, if available. Returns an empty list, if not.
             * @return Enumeration of allowed field values, if available, and an empty list otherwise.
             */
            QStringList getEnumeration();

            /**
             * @brief getItemType Convenience function for retrieving the Item Type restriction of this type, if available. Returns an empty string, if not.
             * @return Item Type of this list, if available, and an empty string otherwise.
             */
            QString getItemType();

            bool isEnumeration();
            bool isList();

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

        private:
            static const QString RestrictionEnumeration;
            static const QString RestrictionItemType;
    };
}

#endif // CUSTOMFIELDTYPE_H
