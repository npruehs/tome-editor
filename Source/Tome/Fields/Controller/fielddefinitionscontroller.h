#ifndef FIELDDEFINITIONSCONTROLLER_H
#define FIELDDEFINITIONSCONTROLLER_H

#include "../Model/fielddefinitionsetlist.h"


namespace Tome
{
    class FieldDefinitionsController
    {
        public:
            FieldDefinitionsController();

            const FieldDefinition& getFieldDefinition(const QString& id) const;
            const FieldDefinitionSetList& getFieldDefinitionSets() const;
            void setFieldDefinitionSets(FieldDefinitionSetList& model);

        private:
            FieldDefinitionSetList* model;
    };
}

#endif // FIELDDEFINITIONSCONTROLLER_H
