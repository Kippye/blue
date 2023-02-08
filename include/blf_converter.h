#pragma once

#include <blf.hpp>
#include <editor.h>
using namespace blf;

class BLF_Tile : public TemplateObject
{
	public:
		double x;
		double y;
		double z;
		double sizeX;
		double sizeY;
		double angle = DEFAULT_TILE.location.Angle;
		bool collisionsEnabled;
		bool _static;
		double bounce = DEFAULT_TILE.physics.Bounce;
		double density = DEFAULT_TILE.physics.Density;
		double friction = DEFAULT_TILE.physics.Friction;
		String texture;
		int textureMode;
		double textureSizeX = DEFAULT_TILE.visuals.TextureSize.x;
		double textureSizeY = DEFAULT_TILE.visuals.TextureSize.y;
		double r = DEFAULT_TILE.visuals.Color.x * 255;
		double g = DEFAULT_TILE.visuals.Color.y * 255;
		double b = DEFAULT_TILE.visuals.Color.z * 255;
		double opacity = DEFAULT_TILE.visuals.Opacity;
		String tag_1 = "NA";
		String tag_2 = "NA";
		String tag_3 = "NA";
		String tag_4 = "NA";
		String tag_5 = "NA";
		// String tag_6;
		// String tag_7;
		// String tag_8;
		// String tag_9;
		// String tag_10;

		BLF_Tile() {}

		BLF_Tile(double x, double y, double z, double sizeX, double sizeY, double angle, bool collisionsEnabled, bool _static, double bounce, double density, double friction, String texture, int textureMode, double textureSizeX, double textureSizeY, double r, double g, double b, double opacity, String tag_1, String tag_2, String tag_3, String tag_4, String tag_5)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->sizeX = sizeX;
			this->sizeY = sizeY;
			this->angle = angle;
			this->collisionsEnabled = collisionsEnabled;
			this->_static = _static;
			this->bounce = bounce;
			this->density = density;
			this->friction = friction;
			this->texture = texture;
			this->textureMode = textureMode;
			this->textureSizeX = textureSizeX;
			this->textureSizeY = textureSizeY;
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
				{"Angle", &angle, TYPE_DOUBLE},
				{"CollisionsEnabled", &collisionsEnabled, TYPE_BOOL},
				{"Static", &_static, TYPE_BOOL},
				{"Bounce", &bounce, TYPE_DOUBLE},
				{"Density", &density, TYPE_DOUBLE},
				{"Friction", &friction, TYPE_DOUBLE},
				{"Texture", &texture, TYPE_STRING},
				{"TextureMode", &textureMode, TYPE_INT},
				{"TextureSizeX", &textureSizeX, TYPE_DOUBLE},
				{"TextureSizeY", &textureSizeY, TYPE_DOUBLE},
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
