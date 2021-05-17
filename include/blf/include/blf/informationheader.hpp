#pragma once

#include <cstdint>

#include "string.hpp"

namespace blf
{
	struct InformationHeader
	{
		blf::String signature;
		int32_t	major;
		int32_t minor;
		int32_t fix;
	};
}
