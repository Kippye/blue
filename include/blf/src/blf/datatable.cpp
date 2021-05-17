#include "blf/datatable.hpp"

#include <map>
#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>

namespace blf
{
    // in the future, the data table will be able to hold objects.
	const int COMMONTABLE_THRESHOLD = 0;

	void DataTable::addObject(TemplateObject* object)
	{
		m_objectList.push_back(object);
	}

	void DataTable::computeCommonTable(blf::CommonTable& table, const blf::ObjectTable& objectTable)
	{
		// Holds the amount of times a pointer is referenced.
		std::map<TemplateObject*, int> referencedPointers;

		// Holds the common table indexes of each pointer.
		std::map<TemplateObject*, uint64_t> pointerIndexes;

		TemplateObject* object = m_objectList[0];
		for (int i = 0; i < m_objectList.size(); i++)
		{
			object = m_objectList[i];
			ObjectDefinition* objectDefinition = objectTable.getDefinitionFromIdentifier(object->getObjectName());
			ObjectAttribute* attribute = &(objectDefinition->attributes[0]);
			for (int j = 0; j < objectDefinition->attributes.size(); j++)
			{
				attribute = &(objectDefinition->attributes[j]);
				if (attribute->attribType == TYPE_OBJECTREFERENCE)
				{
					void* offset = getOffsetFromPointers(objectDefinition->templatePointer, attribute->offset);
					TemplateObject** location = (TemplateObject**)getPointerFromOffset(object, offset);

					if (referencedPointers.find(*location) 
						== referencedPointers.end())
					{
						referencedPointers.insert({ *location, 1 });
					}
					else
					{
						referencedPointers[*location]++;
					}
				}
			}
		}
		
		int index = 0;
		for (auto& pair : referencedPointers)
		{
			// If the reference count is above the treshold.
			if (pair.second > COMMONTABLE_THRESHOLD)
			{
				//pointerIndexes.insert({ pair.first, index });
				table.addCommonObject((TemplateObject*)pair.first);
				                
				// Removing all of the occurences of the common object from the data table.
				m_objectList.erase(std::remove(m_objectList.begin(), m_objectList.end(), pair.first), m_objectList.end());

				((TemplateObject*)pair.first)->commonTableIndex = index;

				index++;
			}
		}
		
		table.buildCommonObjectArray();
	}

	bool DataTable::isBuilt()
	{
		return m_isBuilt;
	}

	int DataTable::getArraySize()
    {
        return m_arraySize;
    }
	
	void DataTable::buildArray()
	{
		m_arraySize = m_objectList.size();
		m_objectArray = new TemplateObject * [m_arraySize];
		for (int i = 0; i < m_arraySize; i++)
		{
			m_objectArray[i] = m_objectList[i];
			m_isBuilt = true;
		}

		m_objectList.clear();
	}
 
	// Remember, these only return positions IN the BUILT ARRAY!
	TemplateObject** DataTable::begin() const { return &m_objectArray[0]; }

	// Again, remember to build the array or else you're going to have a bad time with segfaults.
	TemplateObject** DataTable::end() const { return &m_objectArray[m_arraySize]; }
}
