#include "blf/writer.hpp"

namespace blf
{
	Writer::Writer(String filePath)
	{
		m_writeStream = new std::ofstream(filePath, std::ios::binary);
	}

	Writer::~Writer()
	{
		m_writeStream->close();
		delete m_writeStream;
	}

	void Writer::writeCommonTable(const CommonTable& table, const ObjectTable& objects)
	{
		// The indexer size goes first.
		write(table.getIndexerSize());

		// The amount of objects stored is second.
		write(table.getArraySize());

		for (CommonTable::ConstIterator it = table.begin(); it != table.end(); it++)
		{
			// Writing the object type
			writeObject(*it, objects, table);
		}
	}
}
