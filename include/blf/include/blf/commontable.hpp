#pragma once

#include <cstdint>
#include <vector>
#include <iterator>

#include "templateobject.hpp"

#include "templatearrayconstiterator.hpp"

namespace blf
{
	/**
	 * The common table is responsible for storing all objects that are 'referenced' inside of a BLF file.
	 */
	class CommonTable
	{
		// The size of the indexer in bytes that will be used in the file.
		// This should be 1 (1-254 objects), 2 (255-65534 objects), 4 (65534-4294967294 objects) and finally 8(4294967294-18446744073709551614 objects)
		uint8_t m_indexerSize;

		// A list of objects that will internally be used to build the m_builtArray.
		std::vector<TemplateObject*> m_objectList;

		// Indicates whether or not we have built the array.
		bool m_isArrayBuilt;

		// The built array is the final array with duplicates taken out,
		// stored as a fixed size array since after the template objects are processed,
		// we don't have a need for the dynamic nature for vectors which will cause small slowdowns anyway.
		TemplateObject** m_builtArray;

		// The size of m_builtArray;
		uint64_t m_arraySize;

		void computeIndexerSize();
		
		void copyFromTable(const CommonTable& otherTable);

		public:
			CommonTable() : m_indexerSize(0), m_objectList(std::vector<TemplateObject*>()), m_isArrayBuilt(false),m_builtArray(nullptr), m_arraySize(0) {};

			CommonTable(const CommonTable& otherTable);

			~CommonTable();

			using ConstIterator = TemplateArrayConstIterator;

			/**
			 *	The size in bytes, of the indexer that will be used to reference objects inside of this table.
			 */
			uint8_t getIndexerSize() const { return m_indexerSize; }

			/**
			 *	Returns whether or not the CommonTable has a usable array of objects already built.
			 */
			bool isArrayBuilt() const { return m_isArrayBuilt; }

			/**
			 *	The size of the internal array.
			 */
			uint64_t getArraySize() const { return m_arraySize; }

			/**
			 *	Deallocates m_builtArray, used in the destructor, and sets m_isBuilt to false.
			 */
			void clearArray();

			/**
			 *	Stores the template object inside of the internal list.
			 *  Make sure that the object is alive during and deallocated only outside this object's life span.
			 */
			void addCommonObject(TemplateObject* obj);

			/**
			 *	Passes the internal array to buildFromExternalList(std::vector<TemplateObject*>*)
			 *
			 *	An array should be cleared before being rebuilt.
			 */
			void buildCommonObjectArray();

			/**
			 *	Builds the internal array of all the common objects used in a specific BLF file.
			 *  Most of what this function does is duplicate deletion, if you ARE sure that you don't need that,
			 *  or you handle it yourself, or you just dont care about every reference creating a new common object,
			 *  use forceListAsArray(std::vector<TemplateObject*>*) to simplify the process considerably.
			 *
			 *  An array should be cleared before being rebuilt.
			 */
			void buildFromExternalList(std::vector<TemplateObject*>* list);

			/**
			 *	Do not use this unless you are absolutely sure of what you are doing, although this function isn't harmful,
			 *  misuse will cause the filesize to increase greatly due to how BLF references work.
			 */
			void forceListAsArray(std::vector<TemplateObject*>* list);
            
            TemplateObject* operator[](const int index) const;

			CommonTable& operator=(const CommonTable& table);

			/* These are to support foreach loops in c++ */
			/* Note: These functions WILL pull from the array, not the internal list. */
			/* if you want control over the list, you can build from an external list. */
			/* Make sure that the array is built before using these! */
			TemplateObject** begin() const;
			TemplateObject** end() const;
	};
}
