#pragma once

#include <cstdint>

namespace blf
{
	enum BLF_TYPE
	{
		TYPE_NULL 				= 0, /** Represents a nullptr */
		TYPE_INT 				= 1, /** Represents an int32_t */
		TYPE_FLOAT 				= 2,
		TYPE_BOOL 				= 4,
		TYPE_DOUBLE 			= 8,
		TYPE_STRING 			= 16,
		TYPE_OBJECTREFERENCE 	= 32, /** The blf version of pointers. */
		TYPE_LONG 				= 64, /**Represents an int64_t*/
	};

    enum BLF_TYPECATEGORY
    {
        CATEGORY_NONE,
        CATEGORY_OBJECTREFERENCE,
        CATEGORY_NUMERICFLOAT,
        CATEGORY_NUMERICINTEGER,
        CATEGORY_STRING,
    };
    
	enum BLF_FLAG
	{
		FLAG_NOTHING 		= 0b0000, /** No flag */
		FLAG_VERBOSE 		= 1, /** Should it print output logs */
		FLAG_TYPESTRICT 	= 2, /** Aborts loading as soon as the file has conflicting object definition types. */
		// Add some more here.

	};

	const int SIZE_DYNAMIC = -1;

	int8_t 				getTypeSize(BLF_TYPE type);
	uint8_t				getAbsoluteTypeSize(BLF_TYPE type);
	
    BLF_TYPECATEGORY 	getTypeCategory(BLF_TYPE type);
}
