#pragma once

#include <vector>

#include "objectattribute.hpp"
#include "foreignattributetable.hpp"

namespace blf
{
	class TemplateObject
	{
		protected:
			ForeignAttributeTable m_foreignAttributes;

		public:
			virtual ~TemplateObject() {};
		
			virtual void* dataPtr() const { return (void*)this; };

			uint64_t commonTableIndex = 0;

			virtual const char* getObjectName() const = 0;
			virtual std::vector<blf::ObjectAttribute> getAttributeMap() = 0;

			// You really shouldn't touch this, unless you're the one that's handling this function.
			// It basically gives you an option whether you want to store attributes that are not a part of your class natively
			// but somehow ended up there anyway, probably through the use of scripts and whatnot.
			// If you don't know what to do, dont override.
			virtual void storeForeignAttributes(ForeignAttributeTable table)
			{
				m_foreignAttributes = table;
			};
	};
}
