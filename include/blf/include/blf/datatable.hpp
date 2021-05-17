#pragma once

#include <vector>
#include <type_traits>

#include "templatearrayconstiterator.hpp"

#include "objecttable.hpp"
#include "commontable.hpp"
#include "templateobject.hpp"
#include "objectdefinition.hpp"

#include "datagroup.hpp"

namespace blf
{
	class DataTable
	{
		public:
			using IndexType = int;

		private:

			std::vector<TemplateObject*> m_objectList;
			TemplateObject** m_objectArray;

			IndexType m_arraySize;
			bool m_isBuilt;

		public:
			using ConstIterator = TemplateArrayConstIterator;

			void addObject(TemplateObject* object);

			/**
			 *	Walks through every object, checks
			 *  for referenced objects, and pushes them onto
			 *  commontable.
			 */
			void computeCommonTable(blf::CommonTable& table, const blf::ObjectTable& object);

			bool 		isBuilt();
            IndexType  	getArraySize();
			void 		buildArray();
            
            template<typename T, typename = std::enable_if_t<std::is_base_of_v<TemplateObject, T>>>
            DataGroup<T> get()
            {
                DataGroup<T> group(m_objectArray);
                T controlObject;
                for( IndexType i = 0; i < m_arraySize; i++)
                {
                    TemplateObject* objPtr = m_objectArray[i];
                    if( strcmp(objPtr->getObjectName(), controlObject.getObjectName()) == 0 )
                    {
                        group.addIndex(i);
                    }
                }
                return group;
            }

			TemplateObject** begin() 	const;
			TemplateObject** end() 		const;
	};
}
