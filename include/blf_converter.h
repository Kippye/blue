#pragma once

#include <blf.hpp>
using namespace blf;

class BLF_Tile : public TemplateObject
{
	public:
		double x;
		double y;
		double z;
		double sizeX;
		double sizeY;
		bool collisionsEnabled;
		bool _static;
		String texture;
		int textureMode;
		double r;
		double g;
		double b;
		double opacity;
		String tag_1;
		String tag_2;
		String tag_3;
		String tag_4;
		String tag_5;
		// String tag_6;
		// String tag_7;
		// String tag_8;
		// String tag_9;
		// String tag_10;

		BLF_Tile() {}

		BLF_Tile(double x, double y, double z, double sizeX, double sizeY, bool collisionsEnabled, bool _static, String texture, int textureMode, double r, double g, double b, double opacity, String tag_1, String tag_2, String tag_3, String tag_4, String tag_5)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->sizeX = sizeX;
			this->sizeY = sizeY;
			this->collisionsEnabled = collisionsEnabled;
			this->_static = _static;
			this->texture = texture;
			this->textureMode = textureMode;
			this->r = r;
			this->g = g;
			this->b = b;
			this->opacity = opacity;
			this->tag_1 = tag_1;
			this->tag_2 = tag_2;
			this->tag_3 = tag_3;
			this->tag_4 = tag_4;
			this->tag_5 = tag_5;
			// this->tag_6 = tag_6;
			// this->tag_7 = tag_7;
			// this->tag_8 = tag_8;
			// this->tag_9 = tag_9;
			// this->tag_10 = tag_10;
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
				{"SizeX", &sizeX, TYPE_DOUBLE},
				{"SizeY", &sizeY, TYPE_DOUBLE},
				{"CollisionsEnabled", &collisionsEnabled, TYPE_BOOL},
				{"Static", &_static, TYPE_BOOL},
				{"Texture", &texture, TYPE_STRING},
				{"TextureMode", &textureMode, TYPE_INT},
				{"R", &r, TYPE_DOUBLE},
				{"G", &g, TYPE_DOUBLE},
				{"B", &b, TYPE_DOUBLE},
				{"Opacity", &opacity, TYPE_DOUBLE},
				{"Tag_1", &tag_1, TYPE_STRING},
				{"Tag_2", &tag_2, TYPE_STRING},
				{"Tag_3", &tag_3, TYPE_STRING},
				{"Tag_4", &tag_4, TYPE_STRING},
				{"Tag_5", &tag_5, TYPE_STRING}
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
		bool load_tag(String tag, int index);
};
