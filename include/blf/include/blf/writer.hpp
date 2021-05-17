#pragma once

#include <fstream>

#include "string.hpp"
#include "datatable.hpp"
#include "commontable.hpp"
#include "objecttable.hpp"
#include "templateobject.hpp"
#include "dynamicwriteable.hpp"

#include "informationheader.hpp"

namespace blf
{
	class Writer
	{
		std::ofstream* m_writeStream;
		//ObjectTable* m_objectTable;
		//CommonTable* m_commonTable;

		public:

			template<typename T>
			void write(T data)
			{
				static_assert(std::is_pod<T>::value, "Read only supports POD types.");
				m_writeStream->write(reinterpret_cast<char*>(&data), sizeof(T));
			}

			template<typename T>
			void dynamicWrite(T* value)
			{
				blf::DynamicWriteable<T> writeable(value);
				writeable.write(m_writeStream);
			}

			void storeObjectTable(ObjectTable* objectTable)
			{
				//m_objectTable = objectTable;
			}

			void storeCommonTable(CommonTable* commonTable)
			{
				//m_commonTable = commonTable;
			}

			void writeIndexer(uint64_t index, uint8_t size)
			{
				switch (size)
				{
					case 1:
						write((uint8_t)index);
                        break;

					case 2:
						write((uint16_t)index);
                        break;

					case 4:
						write((uint32_t)index);
                        break;

					case 8:
						write(index);
                        break;
				}
			}

			void writeInformationHeader(const InformationHeader& header)
			{
				m_writeStream->write(header.signature.getBuffer(), 4);
				write(header.major);
				write(header.minor);
				write(header.fix);
			}

			void writeObjectTable(const ObjectTable& table)
			{
				uint8_t indexerSize = table.getIndexerSize();
				int64_t size = table.getSize();
				//m_writeStream->write(reinterpret_cast<char*>(&indexerSize), sizeof(indexerSize));
				//m_writeStream->write(reinterpret_cast<char*>(&size), sizeof(size));
				write(indexerSize);
				write(size);
				for (int i = 0; i < size; i++)
				{
					blf::ObjectDefinition* definition = table.getDefinitionFromIndex(i);
					dynamicWrite(&definition->identifier);
					uint8_t attributeCount = definition->attributes.size();
					write(attributeCount);
					for (int j = 0; j < attributeCount; j++)
					{
						ObjectAttribute* attribute = &(definition->attributes[j]);
						dynamicWrite(&attribute->name);
						uint8_t type = attribute->attribType;
						write(type);
					}
				}
			}

			/**
			 *	Writes the common table to file, i'll take this as another opportunity to say, if for whatever reason you're implementing
			 *	an alternate saving mechanism other than the one that BLF already provides, then please do be careful when using common tables,
			 *  if you don't remove duplicate references from the list before you build it, you will have many duplicates that will take away from the size of the file.
			 *
			 *	You have been warned.
			 */
			void writeCommonTable(const CommonTable& table, const ObjectTable& objects);

			void writeDataTable(const DataTable& dataTable, const ObjectTable& objects, const CommonTable& common)
			{
				for (DataTable::ConstIterator it = dataTable.begin(); it != dataTable.end(); it++)
				{
					write((uint8_t)(0xAA));
					writeObject(*it, objects, common);
				}
				write((uint8_t)255);
			}

			/**
			 *	Writes the object to the file, make sure you stored your defined blf::ObjectTable first, using storeObjectTable(blf::ObjectTable).
			 *	After you've done that, the blf::Writer will manage the rest.
			 */
			void writeObject(const TemplateObject* obj, const ObjectTable& object, const CommonTable& common)
			{
				const char* objectName = obj->getObjectName();
				ObjectDefinition* objectDefinition = object.getDefinitionFromIdentifier(obj->getObjectName());
				// Writing the identifier to the file.
				dynamicWrite(&objectName);

				for (int i = 0; i < objectDefinition->attributes.size(); i++)
				{
					ObjectAttribute* objectAttribute = &objectDefinition->attributes[i];
					void* offset = getOffsetFromPointers(objectDefinition->templatePointer, objectAttribute->offset);
					void* location = getPointerFromOffset(obj->dataPtr(), offset);
					int8_t size = getTypeSize(objectAttribute->attribType);

					dynamicWrite(&objectAttribute->name);
					if (size != -1)
					{
						m_writeStream->write((char*)location, size);
					}
					else
					{
                        if( objectAttribute->attribType == TYPE_STRING )
                        {
                            dynamicWrite(((String*)location));
                        }
                        else if(objectAttribute->attribType == TYPE_OBJECTREFERENCE)
                        {
								TemplateObject* object = (TemplateObject*)location;
								writeIndexer(object->commonTableIndex, common.getIndexerSize());
						}
					}
				}
			}

			Writer(blf::String filePath);
			~Writer();

			/**
			 *	Returns the internal writer stream used.
			 */
			std::ofstream& getStream() { return *m_writeStream; }
	};
}
