#include "blf/foreignattributetable.hpp"

namespace blf
{
    void ForeignAttributeTable::addDefinition(ObjectAttribute definition)
    {
        attributes.push_back(definition);
    }
}