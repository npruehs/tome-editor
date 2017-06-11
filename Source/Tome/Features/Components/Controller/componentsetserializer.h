#ifndef COMPONENTSETSERIALIZER_H
#define COMPONENTSETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class ComponentSet;

    /**
     * @brief Reads and writes components from any device.
     */
    class ComponentSetSerializer
    {
        public:
            /**
             * @brief Constructs a new serializer for reading and writing components from any device.
             */
            ComponentSetSerializer();

            /**
             * @brief Writes the passed component set to the specified device.
             * @param device Device to write the component set to.
             * @param componentSet Component set to write.
             */
            void serialize(QIODevice& device, const ComponentSet& componentSet) const;

            /**
             * @brief Reads the component set from the specified device.
             * @param device Device to read the component set from.
             * @param componentSet Component set to read the data into.
             */
            void deserialize(QIODevice& device, ComponentSet& componentSet) const;

        private:
            static const QString ElementComponents;
            static const QString ElementName;
    };

}


#endif // COMPONENTSETSERIALIZER_H
