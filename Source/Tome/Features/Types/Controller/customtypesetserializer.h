#ifndef CUSTOMTYPESETSERIALIZER_H
#define CUSTOMTYPESETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class CustomTypeSet;

    class CustomTypeSetSerializer
    {
        public:
            CustomTypeSetSerializer();

            void serialize(QIODevice& device, const CustomTypeSet& customTypeSet) const;
            void deserialize(QIODevice& device, CustomTypeSet& customTypeSet) const;

        private:
            static const QString AttributeKey;
            static const QString AttributeValue;
            static const QString ElementName;
            static const QString ElementRestriction;
            static const QString ElementRestrictions;
            static const QString ElementType;
            static const QString ElementTypes;
    };
}

#endif // CUSTOMTYPESETSERIALIZER_H