#pragma once

#include "enums.hpp"
#include "string.hpp"

#include <iostream>

namespace blf
{
	struct ObjectAttribute
	{
		blf::String name;
		void* offset;
		BLF_TYPE attribType;

		/**
		 *	ID/Name of the TemplateObject referenced by the reference.
		 *  Will only be used if blf::BLF_TYPE is blf::TYPE_OBJECTREFERENCE
		 */
		const char* referencedType = nullptr; 

		/**
		 *	Only used by blf::Reader.
		 */
		bool isActive = false;

		/**
		 *	Marks whether or not the attribute doesn't exist in the reader's object table, 
		 *	and only in the file's object table.
		 */
		bool isForeign = false;
	};

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
	void* getOffsetFromPointers(	  void* base, 		void* 	relativeAttribute);
	void* getOffsetFromPointers(const void* base, const void* 	relativeAttribute);

	/**
	 *	Does the reverse of @ref getAttributeLocation(void*, void*), which means it takes in an offset and gives you a pointer.
	 */
	void* getPointerFromOffset(		 void* 	base, 		void* 	location);
	void* getPointerFromOffset(const void* 	base, const void* 	location);
}