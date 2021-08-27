#include <blf_converter.h>
#include <main.h>
#include <iostream>
#include <chrono>

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
	program.render.instanceAdditionalData.clear();
	program.editor.tiles.clear();

	for (BLF_Tile* tile : tiles)
	{
		std::string texturePath = tile->texture;
		//std::cout << texturePath << std::endl;
		if (program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath) == glm::vec2(-1, -1))
		{
			// TODO: handle this shit
		}
		// TODO: load more tile options
		program.editor.add_tile(program.editor.tiles.emplace_back(Location(glm::vec4(tile->x, tile->y, tile->z, 0.0), glm::vec3(tile->sizeX, tile->sizeY, 1.0f)), Physics(tile->collisionsEnabled, tile->_static), Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath), texturePath, (TEXTUREMODE)tile->textureMode)));
	}

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

	std::vector<E_Tile>& editor_tiles = program.editor.tiles;

	std::cout << "(BLF) Writing " << editor_tiles.size() << " tiles." << std::endl;

	// dunno if i need this: if (editor_tiles.size() == 0) { std::cerr << "Tried to write 0 tiles" << std::endl; return; }

    tiles.reserve(editor_tiles.size());

	for (int i = 0; i < editor_tiles.size(); i++)
	{
		std::cout << program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_tiles[i].visuals.atlasCoords) << std::endl;
		tiles.emplace_back(
            editor_tiles[i].location.Position.x,
            editor_tiles[i].location.Position.y,
            editor_tiles[i].location.Position.z,
			editor_tiles[i].location.Size.x,
            editor_tiles[i].location.Size.y,
            editor_tiles[i].physics.CollisionsEnabled,
            editor_tiles[i].physics.Static,
            program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_tiles[i].visuals.atlasCoords),
            (int)editor_tiles[i].visuals.TextureMode
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
