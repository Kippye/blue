#pragma once

#include "string.hpp"
#include "objectattribute.hpp"
#include "templatecreator.hpp"

#include "blf/templateobject.hpp"

#include <vector>

namespace blf
{
	struct ObjectDefinition
	{
		// This is what you use to detect object types in C++ when reading.
		blf::String identifier;
		void* templatePointer = nullptr;

		/**
		 *	If an object is foreign, all it's attributes are also foreign.
		 */
		bool isForeign = false;

		// Takes care of instantiating new instances of the object.
		blf::TemplateCreator* creator;

		/**
		 * A list of blf::ObjectAttribute objects that the object wants to be available.
		 */
		std::vector<ObjectAttribute> attributes;

		/**
		 * Never use this variable yourself, it only should ever be handled
		 * by the blf::Reader. Shows which attributes are active, or in other
		 * words, which ones are defined and supported by the file.
		 */
		//std::vector<int> activeAttributeIndexes;  
        
        uint32_t activeAttributeCount = 0;
	};
    
    ObjectAttribute* getObjectAttributeByIdentifier(ObjectDefinition* definition, String identifier);

	/**
	 * Copies object data to a object definition that can be later managed by blf classes.
	 */
	void initializeDefinition(blf::TemplateObject* obj, blf::ObjectDefinition* definition);

	template<typename T, typename = std::enable_if<std::is_base_of<blf::TemplateObject, T>::value>>
    ObjectDefinition createDefinition()
	{
		ObjectDefinition definition;
		T object;
		initializeDefinition(&object, &definition);
		definition.creator = new SpecialisedTemplateCreator<T>();
		return definition;
	}
}
