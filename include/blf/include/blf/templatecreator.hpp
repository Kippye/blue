#pragma once

#include "templateobject.hpp"

#include <cassert>
#include <type_traits>

namespace blf
{
	class TemplateCreator
	{
		public:
			virtual TemplateObject* createNew() = 0;
	};

	template<typename T, 
		typename = std::enable_if_t<std::is_base_of_v<blf::TemplateObject, T>>>
	class SpecialisedTemplateCreator : public TemplateCreator
	{
		public:
			TemplateObject* createNew() override
			{
				static_assert(std::is_default_constructible<T>::value, "create requires a default constructor");
				return new T();
			}
	};
}