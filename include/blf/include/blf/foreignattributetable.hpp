#pragma once

#include <cstdint>
#include <vector>

#include "objectattribute.hpp"

namespace blf
{
	class ForeignAttributeTable
	{
		std::vector<ObjectAttribute> attributes;
		public:
			void addDefinition(ObjectAttribute attribute);

			const ObjectAttribute* begin() const
			{
				return ((attributes.size() > 0) ? &(attributes[0]) : nullptr);
			}

			const ObjectAttribute* end() const
			{
				return ((attributes.size() > 0) ? &(attributes[attributes.size() - 1]) : nullptr);
			}
	};
}
