#pragma once

#include "string.hpp"
#include "datatable.hpp"
#include "objecttable.hpp"
#include "dynamicreadable.hpp"
#include "foreignobject.hpp"
#include "templateobject.hpp"
#include "commontable.hpp"

#include <fstream>
#include <type_traits>

namespace blf
{
	class Reader
	{
		std::ifstream* readStream;
		bool m_strictTypeChecking = false; // By default type differences end up in warnings.

		public:
			Reader(blf::String filePath, BLF_FLAG readerFlags = FLAG_NOTHING);
			~Reader();

			/**
			 *	Used to read more basic POD types. It reads the sizeof() bytes of the object. 
			 */
			template<typename T>
			T read()
			{
				static_assert(std::is_pod<T>::value, "Read only supports POD types.");
				T newInstance;
				readStream->read(reinterpret_cast<char*>(&newInstance), sizeof(T));

				return newInstance;
			}

			/**
			 *	Used to read more complex types that don't always have the
			 *  same size, such as a const char* or blf::String.
			 *  In order to do this, this function uses blf::DynamicReadable
			 */
			template<typename T>
			T dynamicRead()
			{
				DynamicReadable<T> readable;
				readable.read(readStream);
				return T(readable.getValue());
			}

			uint64_t readIndexer(uint8_t size)
			{
				switch (size)
				{
					case 1:
						return read<uint8_t>();

					case 2:
						return read<uint16_t>();

					case 4:
						return read<uint32_t>();

					case 8:
						return read<uint64_t>();
				}
			}
			
			void readDataTable(DataTable* dataTable, const ObjectTable& objectTable, const CommonTable& commonTable)
            {      
                uint8_t byte = read<uint8_t>();
                while( byte == 0xAA )
                {
                    dataTable->addObject(readObject(objectTable, commonTable.getIndexerSize()));
                    byte = read<uint8_t>();
                }
            }

			void readObjectTable(ObjectTable& definedTable)
			{
				std::vector<ObjectDefinition> definitions;
				definedTable.setIndexerSize(read<uint8_t>());
				size_t size = read<int64_t>();
				for (int i = 0; i < size; i++)
				{
					blf::ObjectDefinition definition; 
					definition.identifier = dynamicRead<String>();
					uint8_t attributeCount = read<uint8_t>();
					for (int j = 0; j < attributeCount; j++)
					{
						ObjectAttribute attribute;
						attribute.name = dynamicRead<String>();
						attribute.attribType = (BLF_TYPE)read<uint8_t>();
						definition.attributes.push_back(attribute);
					}
					definitions.push_back(definition);
				}

				// Looping to check integrity of both tables
				for (int i = 0; i < definitions.size(); i++)
				{
					ObjectDefinition readDefinition = definitions[i];
					ObjectDefinition* nativeDefinition = definedTable.getDefinitionFromIdentifier(readDefinition.identifier);
					
					if (nativeDefinition == nullptr)
					{
						std::cout << "Definition was nullptr, wasn't found, marking as foreign." << std::endl;
						readDefinition.isForeign = true;
						for (ObjectAttribute attribute : readDefinition.attributes)
						{
							attribute.isActive = true;
							attribute.isForeign = true;
							readDefinition.activeAttributeCount++;
						}
						definedTable.insertDefinition(readDefinition);
						continue;
					}

					this->processDefinitionAttributes(nativeDefinition, &definitions[i]);
				}
			}

			void processDefinitionAttributes(ObjectDefinition* nativeDefinition, ObjectDefinition* readDefinition)
			{
				for (int i = 0; i < nativeDefinition->attributes.size(); i++)
				{
					ObjectAttribute nativeAttribute = nativeDefinition->attributes[i];
					bool foundMatchingAttribute = false;
					ObjectAttribute readAttribute;
					int j = 0;
					for (ObjectAttribute fileAttribute : readDefinition->attributes)
					{
						if (fileAttribute.name == nativeAttribute.name)
						{
							foundMatchingAttribute = true;
							readAttribute = fileAttribute;
							readDefinition->attributes[j].isActive = true;
							break;
						}
						j++;
					}

					// If matching definition couldn't be found.
					if (foundMatchingAttribute == false)
					{
						// Warning the user.
						std::cout << "Warning: Expected attribute for name '"
							<< nativeAttribute.name <<
							"' not found in file! Attribute won't be activated." << std::endl;
						continue;
					}

					// Activating attribute as both the file and the program have defined it.
					nativeAttribute.isActive = true;

					BLF_TYPE nativeAttribType = nativeAttribute.attribType;
					BLF_TYPE readAttribType   = readAttribute.attribType;
					if (readAttribType != nativeAttribType)
					{
						std::cout << "File and native definitions differ, opting to use file's attrib type for: "
						<< nativeAttribute.name << std::endl;                                
						if( getTypeCategory(nativeAttribType) == getTypeCategory(readAttribType))
						{
							nativeAttribute.attribType = readAttribType;
							//int sizeDiff = getTypeSize(nativeAttribType) - getTypeSize(readAttribType);
							//nativeAttribute.offset += sizeDiff;
							if( getTypeSize(readAttribType) > getTypeSize(nativeAttribType))
							{
								std::cout << "File definition type is larger than native type! Opting to mark as foreign in order to prevent memory errors" << std::endl;
								nativeAttribute.isForeign = true;
							}
						}
						else
						{
							std::cout << "FATAL: Fatal type mismatch detected! Marking attribute as foreign in order to avoid errors" << std::endl;
							nativeAttribute.isForeign = true;
						}
					}

					nativeDefinition->attributes[i] = nativeAttribute;
				}

				for (ObjectAttribute attrib : readDefinition->attributes)
				{
					if (attrib.isActive == false)
					{
						std::cout << "Assuming attribute '" << attrib.name << "' is foreign!" << std::endl;
                        attrib.isForeign = true;
                        nativeDefinition->attributes.push_back(attrib);
                    }
                    else
                    {
                        nativeDefinition->activeAttributeCount++;
                    }
				}
			}
			
			void readAttribute(ObjectAttribute* objectAttribute, int commonTableIndexerSize, void* addr, ForeignAttributeTable* foreignAttributes)
            {
                int8_t size = getTypeSize(objectAttribute->attribType);
                if( objectAttribute->isForeign == true ) 
                { 
                    void* foreignLoc = nullptr;
                    if (size != SIZE_DYNAMIC)
                    {
                        foreignLoc = new char[size];
                        readStream->read((char*)foreignLoc, size);
                    }
                    else
                    {
                        if (objectAttribute->attribType == TYPE_STRING)
                        {
                            foreignLoc = new char[sizeof(String)];
                            *((String*)foreignLoc) = dynamicRead<String>();
                        }							
                        else if(objectAttribute->attribType == TYPE_OBJECTREFERENCE)
                        {
                            int commonTableReference = readIndexer(commonTableIndexerSize);
                            foreignLoc = new char[sizeof(commonTableReference)];
                            *((int*)foreignLoc) = commonTableReference;
                        }
                    }
                    foreignAttributes->addDefinition({objectAttribute->name, foreignLoc, objectAttribute->attribType, objectAttribute->referencedType});
                    return; 
                }
				if (size != SIZE_DYNAMIC)
				{
					readStream->read((char*)addr, size);
				}
				else
				{
					if (objectAttribute->attribType == TYPE_STRING)
					{
						String str = dynamicRead<String>();
							
						(*(blf::String*)addr) = str;
					}
					else if (objectAttribute->attribType == TYPE_OBJECTREFERENCE)
					{
						int commonTableReference = readIndexer(commonTableIndexerSize);
						*((int*)addr) = commonTableReference;
					}
				}
            }

			TemplateObject* readObject(const ObjectTable& objectTable, uint8_t commonTableIndexerSize)
			{
				// Reading the name of the object (used as a reference to the object table)
				const char* objectName = dynamicRead<const char*>();

				ObjectDefinition* objectDefinition = objectTable.getDefinitionFromIdentifier(objectName);	

				if (objectDefinition == nullptr)
				{
					//	return nullptr;
					std::cout << "Fatal! Definition for '" << objectName << "'" << std::endl;
					return nullptr;
				}

				bool foreign = objectDefinition->isForeign;

				ForeignAttributeTable foreignAttributes;

				TemplateObject* obj = (foreign) ? new ForeignObject(objectName, objectDefinition->attributes) : objectDefinition->creator->createNew();

				for (int i = 0; i < objectDefinition->activeAttributeCount; i++)
				{
                    blf::String attributeName = dynamicRead<blf::String>();
					ObjectAttribute* objectAttribute = getObjectAttributeByIdentifier(objectDefinition, attributeName);
                    
                    void* offset = getOffsetFromPointers(objectDefinition->templatePointer, objectAttribute->offset);
                    void* location = getPointerFromOffset(obj->dataPtr(), offset);
                    
                    readAttribute(objectAttribute, commonTableIndexerSize, location, &foreignAttributes);
                }

				obj->storeForeignAttributes(foreignAttributes);

				return obj;
			}

			CommonTable readCommonTable(const ObjectTable& objectTable)
			{
				CommonTable table;

				uint8_t indexerSize = read<uint8_t>();

				uint64_t tableSize = read<uint64_t>();

				for (int i = 0; i < tableSize; i++)
				{
					blf::TemplateObject* object = readObject(objectTable, indexerSize);
					table.addCommonObject(object);
				}
				return table;
			}

			std::ifstream& getStream() { return *readStream; }
	};
}
