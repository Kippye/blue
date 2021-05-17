#include "blf/reader.hpp"

namespace blf
{
	Reader::Reader(blf::String filePath, BLF_FLAG flag)
	{
		readStream = new std::ifstream(filePath, std::ios::binary);
	}

	Reader::~Reader()
	{
		readStream->close();
		delete readStream;
	}
}