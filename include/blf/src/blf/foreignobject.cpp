#include "blf/foreignobject.hpp"

#include "blf/enums.hpp"
#include "blf/objectattribute.hpp"

#include <cstring>

namespace blf
{
    ForeignObject::ForeignObject(const char* nameString, std::vector<ObjectAttribute>& attributes)  
    {
        size_t stringLength = strlen(nameString);
        name = new char[stringLength + 1];
        std::memcpy(name, nameString, sizeof(char) * stringLength);
        name[stringLength] = '\0';

        size_t totalSize = 0;

        for(ObjectAttribute attr : attributes)
        {
            totalSize += getAbsoluteTypeSize(attr.attribType);
        }

        this->data = new char[totalSize];

        totalSize = 0;

        attributeMap = attributes;

        for(int i = 0; i < attributes.size(); i++)
        {      
            attributes[i].isForeign = false;
            attributes[i].offset = (void*)totalSize;
            attributeMap[i].isForeign = false;
            attributeMap[i].offset = (void*)(&data[totalSize]);
            totalSize += getAbsoluteTypeSize(attributeMap[i].attribType);    
        }
    };
}