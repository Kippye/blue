#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include <initializer_list>

#include "string.hpp"		
#include "objectdefinition.hpp"

namespace blf
{
	class ObjectTable
	{
		uint8_t m_indexerSize;
		size_t m_size = 0;
		ObjectDefinition** m_definitions;
		bool m_empty = true;

		void copyFromTable(const ObjectTable& otherTable)
		{
			m_indexerSize	= otherTable.m_indexerSize;
			m_empty			= otherTable.m_empty;
			m_size			= otherTable.m_size;
			m_definitions	= new ObjectDefinition * [m_size];

			for (int i = 0; i < m_size; i++)
			{
				m_definitions[i] = new ObjectDefinition();
				(*m_definitions[i]) = (*otherTable.m_definitions[i]);
			}
		};

		public:

			ObjectTable() {}

			// COPY CONSTRUCTOR, COPY CONSTRUCTOR, NEEDED, IT IS.
			ObjectTable(const ObjectTable& table)
			{
				copyFromTable(table);
			}

			ObjectTable(std::initializer_list<ObjectDefinition> definitions)
			{
				setContent(definitions);
			}

			ObjectTable& operator =(const ObjectTable& otherTable)
			{
				copyFromTable(otherTable);
				return *this;
			}

			~ObjectTable()
			{
				if( !m_empty )
				{
					for (int i = 0; i < m_size; i++)
					{
						delete m_definitions[i];
					}
					delete[] m_definitions;
				}
			}

			size_t getSize() const { return m_size; }
			uint8_t getIndexerSize() const { return m_indexerSize; }

			// Don't touch these unless you know what you're doing
			void setIndexerSize(uint8_t indexerSize) { m_indexerSize = indexerSize; }
			void setContent(std::initializer_list<ObjectDefinition> definitions)
			{
				m_empty = false;
				m_size = definitions.size();
				m_definitions = new ObjectDefinition * [m_size];

				int i = 0;
				for (ObjectDefinition definition : definitions)
				{
					m_definitions[i] = new ObjectDefinition();
					for (ObjectAttribute attribute : definition.attributes)
					{
						ObjectAttribute nAttr = attribute;
						nAttr.attribType = attribute.attribType;
						m_definitions[i]->attributes.push_back(nAttr);
					}
					m_definitions[i]->identifier = definition.identifier;
					m_definitions[i]->templatePointer = definition.templatePointer;
					m_definitions[i]->creator = definition.creator;
					m_definitions[i]->activeAttributeCount = definition.activeAttributeCount;
					//m_definitions[i]->activeAttributeIndexes = definition.activeAttributeIndexes;
					m_definitions[i]->isForeign = definition.isForeign;
					i++;
				}
			};

			/*
			 *	Should be called after setContent was.
			 */
			void insertDefinition(ObjectDefinition definition)
			{
				size_t oldSize = m_size;

				m_empty = false;
				m_size++;

				ObjectDefinition** oldDefinitions = m_definitions;
				m_definitions = new ObjectDefinition*[m_size];

				// Copying the array over.
				for (int i = 0; i < m_size; i++)
				{
					(m_definitions[i]) = (oldDefinitions[i]);
				}

				ObjectDefinition* defPtr = new ObjectDefinition();
				for (ObjectAttribute attribute : definition.attributes)
				{
					defPtr->attributes.push_back(attribute);
				}
				defPtr->identifier = definition.identifier;
				defPtr->templatePointer = definition.templatePointer;
				defPtr->creator = definition.creator;
				defPtr->isForeign = definition.isForeign;
				defPtr->activeAttributeCount = definition.activeAttributeCount;

				delete[] oldDefinitions;


				m_definitions[oldSize] = defPtr;
			}

			ObjectDefinition* getDefinitionFromIndex(int index) const
			{
				return m_definitions[index];
			}

			ObjectDefinition* getDefinitionFromIdentifier(blf::String identifier) const
			{
				ObjectDefinition* definition = m_definitions[0];
				for (int i = 0; i < m_size; i++, definition = m_definitions[i])
				{
					if (identifier == definition->identifier)
					{
						return definition;
					}
				}

				std::cout << "Warning! Definition '" << identifier << "' could not be found!" << std::endl;
				return nullptr;
			}
	};
}