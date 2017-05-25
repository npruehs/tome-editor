#ifndef CUSTOMTYPESETSERIALIZER_H
#define CUSTOMTYPESETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class CustomTypeSet;

    /**
     * @brief Reads and writes custom types from any device.
     */
    class CustomTypeSetSerializer
    {
        public:
            /**
             * @brief Writes the passed custom type set to the specified device.
             * @param device Device to write the custom type set to.
             * @param customTypeSet Custom type set to write.
             */
            void serialize(QIODevice& device, const CustomTypeSet& customTypeSet) const;

            /**
             * @brief Reads the passed custom type set from the specified device.
             * @param device Device to read the custom type set from.
             * @param customTypeSet Custom type set to fill.
             */
            void deserialize(QIODevice& device, CustomTypeSet& customTypeSet) const;

        private:
            static const QString AttributeKey;
            static const QString AttributeValue;
            static const QString AttributeVersion;
            static const QString AttributeVersionDeprecated;
            static const QString ElementConstrainingFacet;
            static const QString ElementConstrainingFacets;
            static const QString ElementFundamentalFacet;
            static const QString ElementFundamentalFacets;
            static const QString ElementName;
            static const QString ElementRestriction;
            static const QString ElementRestrictions;
            static const QString ElementType;
            static const QString ElementTypes;

            static const int Version;
    };
}

#endif // CUSTOMTYPESETSERIALIZER_H
