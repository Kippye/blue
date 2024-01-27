#include <blf_converter.h>
#include <util.h>
#include <main.h>
#include <iostream>
#include <chrono>
#include <filesystem>

class Program;
extern Program program;

void BLF_Converter::load_file(const char* path)
{
	std::cout << "(BLF) Loading of file at " << path << " requested." << std::endl;

	// READING
	auto start_time = std::chrono::high_resolution_clock::now();

	BLFFile readFile;

	ObjectTable objects = {
		createDefinition<BLF_Tile>(),
	};

	blf::readFile(path, readFile, objects);

	// we're not gonna copy because we're retarded :D
	DataTable& data = readFile.data;
	DataGroup<BLF_Tile> tiles = data.get<BLF_Tile>();

	program.render.instanceTransformData.clear();
	program.render.instanceTextureData.clear();
	program.render.instanceAtlasData.clear();
	program.render.instanceColorData.clear();
	program.render.instanceAdditionalData.clear();
	program.editor.clear_tags();
	program.editor.tiles.clear();
	program.editor.selection.clear();

	for (BLF_Tile* tile : tiles)
	{
		std::string texturePath = std::string((const char*)tile->texture);
		//std::cout << "load_file: loaded tile's texture: " << texturePath << std::endl;
		if (program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath) == glm::uvec4(0))
		{
			// TODO: handle this shit
		}

		bool tags[MAX_TAGS] = {
			load_tag(tile->tag_1, 0),
			load_tag(tile->tag_2, 1),
			load_tag(tile->tag_3, 2),
			load_tag(tile->tag_4, 3),
			load_tag(tile->tag_5, 4)
		};

		program.editor.add_tile(program.editor.tiles.emplace_back(
			Location(glm::vec4(tile->x, tile->y, tile->z, 0.0), glm::vec3(tile->sizeX, tile->sizeY, 1.0f), tile->angle), 
			Physics(tile->collisionsEnabled, tile->_static, tile->bounce, tile->density, tile->friction), 
			Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath), texturePath, (TEXTUREMODE)tile->textureMode, glm::vec2(tile->textureSizeX, tile->textureSizeY), glm::vec4(tile->r / 255.0f, tile->g / 255.0f, tile->b / 255.0f, tile->opacity), tile->opacity),
			tags
		));
	}

	// update title
	std::filesystem::path blfPath = std::filesystem::path(path);
	program.windowManager.setTitle(blfPath.filename().string().c_str());

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cout << "(BLF) File read in: " << duration << " milliseconds." << std::endl;
}

void BLF_Converter::write_file(const char* path)
{
	// WRITING
	std::vector<BLF_Tile> tiles;

	DataTable data;

	ObjectTable objects = {
		createDefinition<BLF_Tile>()
	};

	std::deque<E_Tile>& editor_tiles = program.editor.tiles;

	std::cout << "(BLF) Writing " << editor_tiles.size() << " tiles." << std::endl;

	// dunno if i need this: if (editor_tiles.size() == 0) { std::cerr << "Tried to write 0 tiles" << std::endl; return; }

    tiles.reserve(editor_tiles.size());

	for (int i = 0; i < editor_tiles.size(); i++)
	{
		std::cout << program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_tiles[i].visuals.atlasLocation) << std::endl;
		tiles.emplace_back(
            editor_tiles[i].location.Position.x,
            editor_tiles[i].location.Position.y,
            editor_tiles[i].location.Position.z,
			editor_tiles[i].location.Size.x,
            editor_tiles[i].location.Size.y,
			editor_tiles[i].location.Angle,
            editor_tiles[i].physics.CollisionsEnabled,
            editor_tiles[i].physics.Static,
            editor_tiles[i].physics.Bounce,
            editor_tiles[i].physics.Density,
            editor_tiles[i].physics.Friction,
            program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_tiles[i].visuals.atlasLocation),
            (int)editor_tiles[i].visuals.TextureMode,
			editor_tiles[i].visuals.TextureSize.x,
			editor_tiles[i].visuals.TextureSize.y,
			// round colors because blur expects them to be integers (less work for user)
			(int)(editor_tiles[i].visuals.Color.x * 255.0f),
			(int)(editor_tiles[i].visuals.Color.y * 255.0f),
			(int)(editor_tiles[i].visuals.Color.z * 255.0f),
			editor_tiles[i].visuals.Opacity,
			editor_tiles[i].tags[0] == true ? program.editor.tags[0] : "NA",
			editor_tiles[i].tags[1] == true ? program.editor.tags[1] : "NA",
			editor_tiles[i].tags[2] == true ? program.editor.tags[2] : "NA",
			editor_tiles[i].tags[3] == true ? program.editor.tags[3] : "NA",
			editor_tiles[i].tags[4] == true ? program.editor.tags[4] : "NA"
        );

		data.addObject(&(tiles[i]));
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	writeFile(path, objects, data);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	std::cout << "Write completed in: " << duration << " milliseconds." << std::endl;
	program.editor.setDirtiness(false);
}

bool BLF_Converter::load_tag(String tag, int index)
{
	// the loaded tile is probably from a BLF file created prior to tags being added
	if (tag == String(""))
	{
		return false;
	}

	if (tag.operator std::string().compare("NA") != 0)
	{
		program.editor.tags[index] = tag;
		return true;
	}

	return false;
}