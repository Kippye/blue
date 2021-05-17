#pragma once

#include <fstream>
#include <cstring>

#include "string.hpp"
#include "objecttable.hpp"
#include "basewriteable.hpp"
#include "informationheader.hpp"

namespace blf
{
	template<typename T>
	class DynamicWriteable : private BaseWriteable<T>
	{
	public:
		using BaseWriteable<T>::BaseWriteable;
	};

	template<>
	class DynamicWriteable<const char*> : private BaseWriteable<const char*>
	{
	public:
		using BaseWriteable<const char*>::BaseWriteable;

		void write(std::ofstream* stream) override
		{
			int32_t size = strlen(*m_value);
			stream->write(reinterpret_cast<char*>(&size), sizeof(int32_t));
			stream->write((char*)(*m_value), size);
		}
	};

	template<>
	class DynamicWriteable<String> : private BaseWriteable<String>
	{
	public:
		using BaseWriteable<String>::BaseWriteable;

		void write(std::ofstream* stream) override
		{
			int32_t size = m_value->getLength();
			stream->write(reinterpret_cast<char*>(&size), sizeof(size));
			stream->write((char*)m_value->getBuffer(), size);
		}
	};

	template<>
	class DynamicWriteable<InformationHeader> : private BaseWriteable<InformationHeader>
	{
	public:
		using BaseWriteable<InformationHeader>::BaseWriteable;

		void write(std::ofstream* stream) override
		{
			stream->write(m_value->signature, 4);
			stream->write(reinterpret_cast<char*>(&m_value->major), sizeof(m_value->major));
			stream->write(reinterpret_cast<char*>(&m_value->minor), sizeof(m_value->minor));
			stream->write(reinterpret_cast<char*>(&m_value->fix), sizeof(m_value->fix));
		}
	};

	template<>
	class DynamicWriteable<ObjectTable> : private BaseWriteable<ObjectTable>
	{
	public:
		using BaseWriteable<ObjectTable>::BaseWriteable;

		void write(std::ofstream* stream) override
		{
			uint8_t indexerSize = m_value->getIndexerSize();
			int64_t size = m_value->getSize();
			stream->write(reinterpret_cast<char*>(&indexerSize), sizeof(indexerSize));
			stream->write(reinterpret_cast<char*>(&size), sizeof(size));
			for (int i = 0; i < size; i++)
			{
				blf::ObjectDefinition* definition = m_value->getDefinitionFromIndex(i);
				int32_t size = definition->identifier.getLength();
				stream->write(reinterpret_cast<char*>(&size), sizeof(size));
				stream->write((char*)definition->identifier.getBuffer(), size);
				uint8_t attributeCount = definition->attributes.size();
				stream->write(reinterpret_cast<char*>(&attributeCount), sizeof(attributeCount));
				for (int j = 0; j < attributeCount; j++)
				{
					ObjectAttribute* attribute = &(definition->attributes[j]);
					int32_t strSize = attribute->name.getLength();
					stream->write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
					stream->write((char*)attribute->name.getBuffer(), strSize);
					uint8_t type = attribute->attribType;
					stream->write(reinterpret_cast<char*>(&type), sizeof(type));
				}
			}
		}
	};
}
