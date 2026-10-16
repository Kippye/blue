#include <blf_converter.h>
#include <util.h>
#include <main.h>
#include <iostream>
#include <chrono>
#include <filesystem>
#include "textures.h"

using namespace blf::file;

class Program;
extern Program program;

BLF_ObjectTableHandler::BLF_ObjectTableHandler() {
    auto& vec2Definition = BLF_Vector2::addDefinition(m_objectTable);
    auto& vec3Definition = BLF_Vector3::addDefinition(m_objectTable);
    auto& colorDefinition = BLF_Color::addDefinition(m_objectTable);
    auto& tileDefinition = BLF_Tile::addDefinition(
        m_objectTable, &vec3Definition, &vec2Definition, &colorDefinition);
}

blf::st::ObjectTable& BLF_ObjectTableHandler::getObjectTable() {
    return m_objectTable;
}

blf::st::ClassDefinition<BLF_Vector3>* BLF_ObjectTableHandler::getVec3Definition()
    const {
    return dynamic_cast<blf::st::ClassDefinition<BLF_Vector3>*>(
        m_objectTable.getDefinition(BLF_Vector3::getName()));
}
blf::st::ClassDefinition<BLF_Vector2>* BLF_ObjectTableHandler::getVec2Definition()
    const {
    return dynamic_cast<blf::st::ClassDefinition<BLF_Vector2>*>(
        m_objectTable.getDefinition(BLF_Vector2::getName()));
}
blf::st::ClassDefinition<BLF_Color>* BLF_ObjectTableHandler::getColorDefinition()
    const {
    return dynamic_cast<blf::st::ClassDefinition<BLF_Color>*>(
        m_objectTable.getDefinition(BLF_Color::getName()));
}
blf::st::ClassDefinition<BLF_Tile>* BLF_ObjectTableHandler::getTileDefinition()
    const {
    return dynamic_cast<blf::st::ClassDefinition<BLF_Tile>*>(
        m_objectTable.getDefinition(BLF_Tile::getName()));
}

BLF_ObjectTableHandler BLF_Converter::m_tableHandler = BLF_ObjectTableHandler();

void BLF_Converter::load_file(const char* path) {
    std::cout << "(BLF) Loading of file at " << path << " requested." << std::endl;

    // READING
    auto start_time = std::chrono::high_resolution_clock::now();

    FileReadStream stream(path);

    auto file = File::fromData(stream);

    auto fileBody = file.deserializeBody(m_tableHandler.getObjectTable());

    auto blfTiles = fileBody.data.groupby(*m_tableHandler.getTileDefinition());

    program.render.instanceTransformData.clear();
    program.render.instanceTextureData.clear();
    program.render.instanceAtlasData.clear();
    program.render.instanceColorData.clear();
    program.render.instanceAdditionalData.clear();
    program.editor.clear_tags();
    program.editor.tiles.clear();
    program.editor.selection.clear();

    for (const BLF_Tile& blfTile : blfTiles) {
        std::string texturePath = blfTile.texture;
        //std::cout << "load_file: loaded tile's texture: " << texturePath << std::endl;
        glm::uvec4 atlasCoords = TextureLoader::getAtlasTextureCoords(
            program.render.textureAtlas, texturePath);
        if (atlasCoords == glm::uvec4(0)) {
            // TODO: handle this shit
        }

        bool tags[MAX_TAGS] = {};

        for (size_t i = 0; i < MAX_TAGS; i++) {
            if (i < blfTile.tags.size()) {
                tags[i] = load_tag(blfTile.tags[i], i);
            } else {
                // TEMP compatibility - there will always be MAX_TAGS tags
                tags[i] = load_tag("NA", i);
            }
        }
        E_Tile tile = blfTile.getTile(atlasCoords, tags);

        program.editor.add_new_tile(tile);
    }

    // update title
    std::filesystem::path blfPath = std::filesystem::path(path);
    program.editor.setDirtiness(false); // Just loaded a file so nothing's changed
    program.windowManager.setTitle(
        blfPath.filename().string().c_str(), program.editor.getDirtiness());

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
            .count();
    std::cout << "(BLF) File read in: " << duration << " milliseconds."
              << std::endl;
}

void BLF_Converter::write_file(const char* path) {
    auto start_time = std::chrono::high_resolution_clock::now();

    FileWriteStream stream(path);
    DataTable data;

    std::deque<E_Tile>& editorTiles = program.editor.tiles;
    std::vector<BLF_Tile> blfTiles;
    auto tileDefinition = m_tableHandler.getTileDefinition();

    std::cout << "(BLF) Writing " << editorTiles.size() << " tiles." << std::endl;

    blfTiles.reserve(editorTiles.size());

    for (int i = 0; i < editorTiles.size(); i++) {
        std::vector<std::string> tileTags = {};
        for (int j = 0; j < sizeof(editorTiles[i].tags); j++) {
            tileTags.push_back(
                editorTiles[i].tags[j] ? program.editor.tags[j] : "NA");
        }
        blfTiles.emplace_back(
            editorTiles[i],
            TextureLoader::getAtlasTexturePath(
                program.render.textureAtlas, editorTiles[i].visuals.atlasLocation),
            tileTags);

        data.insert(tileDefinition->serialize(blfTiles[i]));
    }

    File file(data, m_tableHandler.getObjectTable());

    file.serialize(stream);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
            .count();

    std::cout << "Write completed in: " << duration << " milliseconds."
              << std::endl;
    program.editor.setDirtiness(false);
}

bool BLF_Converter::load_tag(std::string tag, int index) {
    // the loaded tile is probably from a BLF file created prior to tags being added
    if (tag == std::string("")) {
        return false;
    }

    if (tag.compare("NA") != 0) {
        program.editor.tags[index] = tag;
        return true;
    }

    return false;
}