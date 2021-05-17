#include "blf/enums.hpp"

#include "blf/string.hpp"

namespace blf
{
	int8_t getTypeSize(BLF_TYPE type)
	{
		switch (type)
		{
			case TYPE_NULL:
				return 0;

			case TYPE_INT:
				return 4;

			case TYPE_FLOAT:
				return 4;

			case TYPE_BOOL:
				return 1;

			case TYPE_DOUBLE:
				return 8;

			case TYPE_STRING:
				return SIZE_DYNAMIC;

			case TYPE_OBJECTREFERENCE:
				// The size of this is decided by the size of the common table indexer.
				return SIZE_DYNAMIC;

			case TYPE_LONG:
				return 8;
		}
	}

	uint8_t getAbsoluteTypeSize(BLF_TYPE type)
	{
		switch (type)
		{
			case TYPE_NULL:
				return 0;

			case TYPE_INT:
				return 4;

			case TYPE_FLOAT:
				return 4;

			case TYPE_BOOL:
				return 1;

			case TYPE_DOUBLE:
				return 8;

			case TYPE_STRING:
				return sizeof(String);

			case TYPE_OBJECTREFERENCE:
				// The size of this is decided by the size of the common table indexer.
				return 8;

			case TYPE_LONG:
				return 8;
		}
	}
	
    BLF_TYPECATEGORY getTypeCategory(BLF_TYPE type)
    {
        switch(type)
        {
            case TYPE_NULL:
                return CATEGORY_NONE;
                
            case TYPE_INT:
                return CATEGORY_NUMERICINTEGER;
                
            case TYPE_FLOAT:
                return CATEGORY_NUMERICFLOAT;
                
            case TYPE_BOOL:
                return CATEGORY_NUMERICINTEGER;
                
            case TYPE_DOUBLE:
                return CATEGORY_NUMERICFLOAT;
                
            case TYPE_STRING:
                return CATEGORY_STRING;
                
            case TYPE_OBJECTREFERENCE:
                return CATEGORY_OBJECTREFERENCE;
                
            case TYPE_LONG:
                return CATEGORY_NUMERICFLOAT;
        }
    }
}
