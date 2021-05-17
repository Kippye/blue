#pragma once

#include <vector>

#include "templateobject.hpp"
#include "foreignattributetable.hpp"

namespace blf
{
	/*
	 *	Storage class for objects that do not necessarily have
	 *	a defined way of being read.
	 */
	class ForeignObject : public TemplateObject
	{
		std::vector<ObjectAttribute> attributeMap;

		public:
			ForeignObject(const char* nameString, std::vector<ObjectAttribute>& attributes);
			~ForeignObject() override {};

			char* name;
			char* data;

			void* dataPtr() const override { return (void*)data; }

			const char* getObjectName() const override
			{
				return (const char*)name;
			}

			std::vector<blf::ObjectAttribute> getAttributeMap() override
			{
				return attributeMap;
			}

			void storeForeignAttributes(ForeignAttributeTable table)
			{
				for(ObjectAttribute attribute : table)
				{
					attributeMap.push_back(attribute);
				}
			}
	};
}