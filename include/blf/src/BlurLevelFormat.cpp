#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>

#include <cstdlib>
#include <ctime>

#include "blf/string.hpp"
#include "blf/reader.hpp"
#include "blf/writer.hpp"
#include "blf/datatable.hpp"
#include "blf/objecttable.hpp"
#include "blf/commontable.hpp"
#include "blf/templateobject.hpp"
#include "blf/templatecreator.hpp"
#include "blf/objectattribute.hpp"
#include "blf/objectdefinition.hpp"
#include "blf/foreignattributetable.hpp"

#include "blf/blffile.hpp"


#include "blf/basereadable.hpp"
#include "blf/basewriteable.hpp"
#include "blf/dynamicwriteable.hpp"
#include "blf/dynamicreadable.hpp"

#include "blf/datagroup.hpp"

const int TEST_TILES = 20000;
const char* BLF_TILE_NAME = "Tile";

// Defining STRDUP as different functions depending on the platform.
// This is needed because some windows compilers see strdup as deprecated
// and use _strdup.

#ifdef _WIN32
#define STRDUP(x) _strdup(x)
#else
#define STRDUP(x) strdup(x)
#endif

class Texture : public blf::TemplateObject
{
	public:
		blf::String filePath;

		Texture() { }

		Texture(blf::String filePath)
		{
			this->filePath = filePath;
		}

		const char* getObjectName() const  override
		{
			return "Texture";
		}

		std::vector<blf::ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"filePath", &filePath, blf::TYPE_STRING}
			};
		}

		void storeForeignAttributes(blf::ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (blf::ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}
};

class testObject : public blf::TemplateObject
{
	public:

		testObject() {}

		// Some properties for this test class.
		int aRandomNumber;
		blf::String aString;

		const char* getObjectName() const override
		{
			return "testObject";
		};

		std::vector<blf::ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"aRandomNumber", &aRandomNumber, blf::TYPE_INT},
				{"aString", &aString, blf::TYPE_STRING}
			};
		}

		void storeForeignAttributes(blf::ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (blf::ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}

};

class Tile : public blf::TemplateObject
{
	public:
		blf::String ID;
		double x;
		double y;
		double z;
		Texture* texture;

		Tile() {}

		Tile(blf::String ID, double x, double y, double z, Texture* texture)
		{
			this->ID = ID;
			this->x = x;
			this->y = y;
			this->z = z;
			this->texture = texture;
		}

		const char* getObjectName() const override
		{
			return "Tile";
		};

		std::vector<blf::ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"ID", &ID, blf::TYPE_STRING},
				{"x", &x, blf::TYPE_DOUBLE},
				{"y", &y, blf::TYPE_DOUBLE},
				{"z", &z, blf::TYPE_DOUBLE},
				{"texture", &texture, blf::TYPE_OBJECTREFERENCE, "Texture"}
			};
		}

		void storeForeignAttributes(blf::ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (blf::ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}
};

class StateTile : public Tile
{
	public:

		int state;

		std::vector<blf::ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"x", &x, blf::TYPE_DOUBLE },
				{"state", &state, blf::TYPE_INT},
				{"y", &y, blf::TYPE_DOUBLE},
				{"z", &z, blf::TYPE_DOUBLE},
				{"texture", &texture, blf::TYPE_OBJECTREFERENCE, "Texture"},
				{"ID", &ID, blf::TYPE_STRING},
			};
		}

		using Tile::storeForeignAttributes;
		using Tile::getObjectName;
};



void writetest()
{
	// WRITING
	std::vector<Tile> tiles;

	blf::DataTable data;

	blf::ObjectTable objects = {
		blf::createDefinition<testObject>(),
		blf::createDefinition<Texture>(),
		blf::createDefinition<Tile>(),
	};

	srand(time(0));

	Texture textures[4];
	textures[0] = Texture("grass.png");
	textures[1] = Texture("dirt.png");
	textures[2] = Texture("crate.png");
	textures[3] = Texture("metalblock.png");

	std::cout << "Write test started with " << TEST_TILES << " tile objects." << std::endl;

    tiles.reserve(TEST_TILES);
    
	for (int i = 0; i < TEST_TILES; i++)
	{
		std::string test = "Tile " + std::to_string(i);

		tiles.emplace_back(
            test,
            5,
            6,
            4,
            &textures[rand() % 4]
        );

		data.addObject(&(tiles[i]));
	}

	auto t1 = std::chrono::high_resolution_clock::now();

	blf::writeFile("level.blf", objects, data);
    
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << "Write completed in: " << duration << " milliseconds." << std::endl;

	std::cout << "Common objects:" << std::endl;

}


void readtest()
{
	// READING
	auto t3 = std::chrono::high_resolution_clock::now();

	blf::BLFFile readFile;

	blf::ObjectTable objects = {
		blf::createDefinition<Texture>(),
		blf::createDefinition<StateTile>(),
	};

	blf::readFile("level.blf", readFile, objects);

	auto t4 = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
	std::cout << "Read completed in: " << duration2 << " milliseconds." << std::endl;
	std::cout << "W" << std::endl;
}

/*
	TASKS:
	-	Finish write logic. [Y.D]
	-	Translate that logic to make matching reading logic. [Y.D]

	-	Document all the classes * - keep till last
	-	Move the header defined functions in reader.hpp and writer.hpp into their respective source files.
	-	Make a function that can simplify this entire process. [Y.D]

	-	Create the DataTable class. [Y.D]
	
	-   Debug output, might be useful in the future.

	-	Group read objects by type, allow DataTable to return list of pointers to all objects of a group. [Y.D]
*/

int main()
{
	std::cout << "begin" << std::endl;
	writetest();
	readtest();

	std::cout << "end" << std::endl;
}