#include "blf/commontable.hpp"

#include <algorithm>
#include <iostream>

namespace blf
{
	void CommonTable::copyFromTable(const CommonTable& otherTable)
	{
		m_indexerSize = otherTable.m_indexerSize;
		m_isArrayBuilt = otherTable.m_isArrayBuilt;
		m_arraySize = otherTable.m_arraySize;
		if( !m_isArrayBuilt )
		{
			m_objectList = otherTable.m_objectList;
		}
		else
		{
			m_builtArray = new TemplateObject*[m_arraySize];
			for( size_t i = 0; i < m_arraySize; i++)
			{
				m_builtArray[i] = otherTable.m_builtArray[i];
			}
		}
	}

	CommonTable::CommonTable(const CommonTable& otherTable)
	{
		copyFromTable(otherTable);
	}

	void CommonTable::computeIndexerSize()
	{
		uint64_t size = getArraySize();
		if (size < UINT8_MAX)
		{
			m_indexerSize = 1;
			return;
		}
		else if (size < UINT16_MAX)
		{
			m_indexerSize = 2;
			return;
		}
		else if (size < UINT32_MAX)
		{
			m_indexerSize = 4;
			return;
		}		
		else if (size < UINT64_MAX)
		{
			m_indexerSize = 8;
			return;
		}
	}

	CommonTable::~CommonTable()
	{
		if (m_isArrayBuilt)
		{
			clearArray();
		}
	}

	void CommonTable::clearArray()
	{
		if (m_isArrayBuilt && m_arraySize > 0 )
		{
			delete m_builtArray;
		}
	}

	void CommonTable::addCommonObject(TemplateObject* obj)
	{
		// Puts it in an internal list.
		m_objectList.emplace_back(obj);
	}

	void CommonTable::buildCommonObjectArray()
	{
		buildFromExternalList(&m_objectList);
	}

	void CommonTable::buildFromExternalList(std::vector<TemplateObject*>* list)
	{
		std::vector<TemplateObject*> temporary;
		int size = list->size();
		blf::TemplateObject* object = ((size > 0) ? ((*list)[0]) : nullptr);
		for (int i = 0; i < size; i++, object = (*list)[std::min(i, (size - 1))])
		{
			if (object == nullptr)
			{
				continue;
			}

			// Hasn't been nullptr'd so it has to be a value that is unique.
			// Which means we can safely add it to the temporary replacement vector.
			temporary.push_back(object);

			blf::TemplateObject* object2 = nullptr;
			object2 = (*list)[(size - 1)];

			// Ending at i + 1 basically ensures that we will never go through an index that will include object.
			// It's also pretty clever since any object before i + 1 has definitely been processed and carried over to the next array
			// or disposed as a duplicate and made into a nullptr.
			for (int j = (size - 1); j > i; j--, object2 = (*list)[j])
			{
				// If the object is a nullptr, it's invalid. 
				if (object2 == nullptr)
				{
					// If the object is at the size boundary, we're going to shrink it, since the object is invalid.
					if (j == size - 1)
					{
						size--;
					}
					continue;
				}

				// If both indexers are the same, we don't want to do anything to the value
				if (i == j)
				{
					continue;
				}

				// Seeing as indexers are NOT the same, if the objects are equal, we've got an unnecessary copy!
				if (object2 == object)
				{
					// Time to get rid of such useless pointers.
					// Setting it as nullptr basically means that it isn't worth anyone's time using it.
					(*list)[j] = nullptr;

					// If the object is at the size boundary, we're going to shrink it, since the object is invalid.
					if (j == size - 1)
					{
						size--;
					}
				}
			}
		}
		forceListAsArray(&temporary);
	}

	void CommonTable::forceListAsArray(std::vector<TemplateObject*>* list)
	{
		m_isArrayBuilt = true;
		m_arraySize = list->size();
		if( m_arraySize > 0 )
		{
			m_builtArray = new TemplateObject * [m_arraySize];
		}
		for (uint64_t i = 0; i < m_arraySize; i++)
		{
			m_builtArray[i] = (*list)[i];

			// The reason that index is set to i + 1, because index 0 is treated as null, even if not present in the table itself.
			m_builtArray[i]->commonTableIndex = i + (uint64_t)1;
		}

		computeIndexerSize();
	}

    TemplateObject* CommonTable::operator[](const int index) const
    {
        return m_builtArray[index];
    }

	CommonTable& CommonTable::operator=(const CommonTable& table)
	{
		if( this == &table ) return *this;

		copyFromTable(table);
	}
	
	// Remember, these only return positions IN the BUILT ARRAY!
	TemplateObject** CommonTable::begin() const { return &m_builtArray[0]; }

	// Again, remember to build the array or else you're going to have a bad time with segfaults.
	TemplateObject** CommonTable::end() const { return &m_builtArray[m_arraySize]; }
}
