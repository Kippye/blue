#include "blf/objectattribute.hpp"

namespace blf
{
	// Returns where technically the attribute should be in an object.
	void* getOffsetFromPointers(void* base, void* relativeAttribute)
	{
		/**
		 *	Returns the memory offset of a given property to a given object.
		 *
		 *  Used for setting properties of an object without setting properties explicitly.
		 *  Both pointers should be related to the same instance of the same class!
		 *
		 *	@param base The pointer to the class instance.
		 *  @param relativeAttribute The pointer to the class property. The offset of this to the class pointer will be found.
									 Used in the attribute system for BLF objects.
		 */
		unsigned long long int baseInt = (unsigned long long int)base;
		unsigned long long int relativeInt = (unsigned long long int)relativeAttribute;
		unsigned long long int location = (relativeInt - baseInt);
		return (void*)location;
	}

	void* getOffsetFromPointers(const void* base, const void* relativeAttribute)
	{
		const unsigned long long int baseInt = (const unsigned long long int)base;
		const unsigned long long int relativeInt = (const unsigned long long int)relativeAttribute;
		unsigned long long int location = (relativeInt - baseInt);
		return (void*)location;
	}

	// 
	void* getPointerFromOffset(void* base, void* location)
	{
		/**
		 *	Does the reverse of @ref getAttributeLocation(void*, void*), which means it takes in an offset and gives you a pointer.
		 */
		 // Converting pointers to integers so we can do arithmetic on them.
		unsigned long long int baseInt = (unsigned long long int)base;
		unsigned long long int locationInt = (unsigned long long int)location;
		unsigned long long int pointer = baseInt + locationInt;
		return (void*)pointer;
	}

	void* getPointerFromOffset(const void* base, const void* location)
	{
		const unsigned long long int baseInt = (const unsigned long long int)base;
		const unsigned long long int locationInt = (const unsigned long long int)location;
		unsigned long long int pointer = baseInt + locationInt;
		return (void*)pointer;
	}
}