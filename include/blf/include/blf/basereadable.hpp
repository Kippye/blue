#pragma once

#include <fstream>
#include <iostream>

namespace blf
{
	template<typename T>
	class BaseReadable
	{
		protected:
			T m_value;
		public:
			virtual void read(std::ifstream* stream)
			{
				int32_t size;
				T instance;
				stream->read(reinterpret_cast<char*>(&size), sizeof(size));
				stream->read(reinterpret_cast<char*>(&instance), size);

				this->m_value = instance;
			}

			T getValue()
			{
				return m_value;
			}
	};
}