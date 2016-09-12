#ifndef COMPONENTSETSERIALIZER_H
#define COMPONENTSETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class ComponentSet;

    class ComponentSetSerializer
    {
        public:
            ComponentSetSerializer();

            void serialize(QIODevice& device, const ComponentSet& componentSet) const;
            void deserialize(QIODevice& device, ComponentSet& componentSet) const;

        private:
            static const QString ElementComponents;
            static const QString ElementName;
    };

}


#endif // COMPONENTSETSERIALIZER_H
