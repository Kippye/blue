#pragma once

#include <blf.hpp>
using namespace blf;

class Texture : public blf::TemplateObject
{
	public:
		String filePath;

		Texture() { }

		Texture(String filePath)
		{
			this->filePath = filePath;
		}

		const char* getObjectName() const  override
		{
			return "Texture";
		}

		std::vector<ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"filePath", &filePath, TYPE_STRING}
			};
		}

		void storeForeignAttributes(ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}
};

class BLF_Tile : public TemplateObject
{
	public:
		double x;
		double y;
		double z;
		String texture;

		BLF_Tile() {}

		BLF_Tile(double x, double y, double z, String texture)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->texture = texture;
		}

		const char* getObjectName() const override
		{
			return "BLF_Tile";
		};

		std::vector<ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"X", &x, TYPE_DOUBLE},
				{"Y", &y, TYPE_DOUBLE},
				{"Z", &z, TYPE_DOUBLE},
				{"Texture", &texture, TYPE_STRING}
			};
		}

		void storeForeignAttributes(ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}
};

class BLF_Converter
{
	public:
		void load_file(const char* path);
		void write_file(const char* path);
};
