#include "blf/objectdefinition.hpp"


#include <iostream>

namespace blf
{
    ObjectAttribute* getObjectAttributeByIdentifier(ObjectDefinition* definition, String identifier)
    {
        ObjectAttribute* attribute = &(definition->attributes[0]);
        for (int i = 0; i < definition->attributes.size(); i++, attribute = &(definition->attributes[i]))
        {
            if (identifier == attribute->name)
            {
                return attribute;
            }
        }

        std::cout << "Warning! Attribute '" << identifier << "' could not be found!" << std::endl;
        return nullptr;
    }

    void initializeDefinition(blf::TemplateObject* obj, blf::ObjectDefinition* definition)
	{
		definition->identifier = obj->getObjectName();
		definition->templatePointer = obj;
		definition->attributes = obj->getAttributeMap();
	}


}
