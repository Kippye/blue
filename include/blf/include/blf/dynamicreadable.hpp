#pragma once

#include "string.hpp"
#include "objecttable.hpp"
#include "basereadable.hpp"
#include "informationheader.hpp"

namespace blf
{
	template<typename T>
	class DynamicReadable : private BaseReadable<T>
	{
	public:
		using BaseReadable<T>::BaseReadable;
		using BaseReadable<T>::getValue;
	};

	template<>
	class DynamicReadable<const char*> : private BaseReadable<const char*>
	{
	public:
		using BaseReadable<const char*>::BaseReadable;
		using BaseReadable<const char*>::getValue;

		void read(std::ifstream* stream) override
		{
			int32_t size;
			stream->read(reinterpret_cast<char*>(&size), sizeof(int32_t));

			char* id = new char[size + 1];

			stream->read(&id[0], size);
			id[size] = (char)'\0';

			this->m_value = id;
		};
	};

	template<>
	class DynamicReadable<blf::String> : private BaseReadable<blf::String>
	{
	public:
		using BaseReadable<blf::String>::BaseReadable;
		using BaseReadable<blf::String>::getValue;

		void read(std::ifstream* stream) override
		{
			int32_t size;
			stream->read(reinterpret_cast<char*>(&size), sizeof(int32_t));

			char* id = new char[size + 1];

			stream->read(&id[0], size);
			id[size] = (char)'\0';

			this->m_value = id;
		};
	};

	template<>
	class DynamicReadable<InformationHeader> : private BaseReadable<InformationHeader>
	{
	public:
		using BaseReadable<InformationHeader>::BaseReadable;
		using BaseReadable<InformationHeader>::getValue;

		void read(std::ifstream* stream) override
		{
			int32_t size;

			char sig[5];

			stream->read(&sig[0], 4);
			sig[4] = '\0';

			this->m_value.signature = blf::String((const char*)sig);
			stream->read(reinterpret_cast<char*>(&m_value.major), sizeof(m_value.major));
			stream->read(reinterpret_cast<char*>(&m_value.minor), sizeof(m_value.minor));
			stream->read(reinterpret_cast<char*>(&m_value.fix), sizeof(m_value.fix));
		};
	};
}