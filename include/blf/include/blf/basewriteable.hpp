#pragma once


namespace blf
{
	template<typename T>
	class BaseWriteable
	{
		protected:
			T* m_value;
		public:
			BaseWriteable(T* instance)
			{
				this->m_value = instance;
			}

			virtual void write(std::ofstream* stream)
			{
				int32_t size = sizeof(*m_value);
				stream->write(reinterpret_cast<char*>(&size), sizeof(size));
				stream->write(reinterpret_cast<char*>(m_value), size);
			}
	};
}
