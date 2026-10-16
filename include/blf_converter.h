#pragma once

#include <blf.hpp>
#include "blf/serialization/api_static.hpp"
#include <editor_tile.h>
using namespace blf;

struct BLF_Vector3 {
        static std::string getName() {
            return "BLF_Vector3";
        }

        float x;
        float y;
        float z;

        BLF_Vector3() {
        }
        BLF_Vector3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        BLF_Vector3(glm::vec3 vec) {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
        }
        BLF_Vector3(glm::vec4 vec) {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
        }

        glm::vec3 getVec3() const {
            return glm::vec3(x, y, z);
        }

        glm::vec4 getVec4(float defaultW = .0f) const {
            return glm::vec4(x, y, z, defaultW);
        }

        static auto& addDefinition(blf::st::ObjectTable& objectTable) {
            return objectTable.define<BLF_Vector3>(
                getName(),
                field("X", &BLF_Vector3::x),
                field("Y", &BLF_Vector3::y),
                field("Z", &BLF_Vector3::z));
        }
};

struct BLF_Vector2 {
        static std::string getName() {
            return "BLF_Vector2";
        }

        float x;
        float y;

        BLF_Vector2() {
        }
        BLF_Vector2(float x, float y) {
            this->x = x;
            this->y = y;
        }
        BLF_Vector2(glm::vec2 vec) {
            this->x = vec.x;
            this->y = vec.y;
        }

        glm::vec2 getVec2() const {
            return glm::vec2(x, y);
        }

        glm::vec3 getVec3(double defaultZ = .0f) const {
            return glm::vec3(x, y, defaultZ);
        }

        static auto& addDefinition(blf::st::ObjectTable& objectTable) {
            return objectTable.define<BLF_Vector2>(
                getName(),
                field("X", &BLF_Vector2::x),
                field("Y", &BLF_Vector2::y));
        }
};

/// RGBA color (0-255)
struct BLF_Color {
        static std::string getName() {
            return "BLF_Color";
        }

        float r;
        float g;
        float b;
        float a;

        BLF_Color() {
        }
        BLF_Color(float r, float g, float b, float a = 255.0f) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
        BLF_Color(glm::vec3 color) {
            this->r = color.x;
            this->g = color.y;
            this->b = color.z;
            this->a = 1;
        }
        BLF_Color(glm::vec4 color) {
            this->r = color.x;
            this->g = color.y;
            this->b = color.z;
            this->a = color.w;
        }

        glm::vec3 getVec3() const {
            return glm::vec3(r, g, b);
        }

        glm::vec4 getVec4() const {
            return glm::vec4(r, g, b, a);
        }

        static auto& addDefinition(blf::st::ObjectTable& objectTable) {
            return objectTable.define<BLF_Color>(
                getName(),
                field("R", &BLF_Color::r),
                field("G", &BLF_Color::g),
                field("B", &BLF_Color::b),
                field("A", &BLF_Color::a));
        }
};

struct BLF_Tile {
        static std::string getName() {
            return "BLF_Tile";
        }

        BLF_Vector3 position;
        BLF_Vector2 size;
        double angle = E_Tile::getDefaultOptions().location.Angle;
        bool collisionsEnabled;
        bool isStatic;
        double bounce = E_Tile::getDefaultOptions().physics.Bounce;
        double density = E_Tile::getDefaultOptions().physics.Density;
        double friction = E_Tile::getDefaultOptions().physics.Friction;
        std::string texture;
        int textureMode;
        BLF_Vector2 textureSize;
        BLF_Color color;
        double opacity = E_Tile::getDefaultOptions().visuals.Opacity;
        std::vector<std::string> tags = {};

        BLF_Tile() {
        }
        BLF_Tile(
            const E_Tile& tile,
            const std::string& textureName,
            const std::vector<std::string>& tileTags) {
            position = BLF_Vector3(tile.location.Position);
            size = BLF_Vector2(tile.location.Size);
            angle = tile.location.Angle;
            collisionsEnabled = tile.physics.CollisionsEnabled;
            isStatic = tile.physics.Static;
            bounce = tile.physics.Bounce;
            density = tile.physics.Density;
            friction = tile.physics.Friction;
            texture = textureName;
            textureMode = tile.visuals.TextureMode;
            textureSize = BLF_Vector2(tile.visuals.TextureSize);
            color = BLF_Color(
                (int)(tile.visuals.Color.x * 255.0f),
                (int)(tile.visuals.Color.y * 255.0f),
                (int)(tile.visuals.Color.z * 255.0f));
            opacity = tile.visuals.Opacity;
            tags = tileTags;
        }

        E_Tile getTile(glm::uvec4 atlasLocation, bool* activeTags) const {
            return E_Tile(
                Location(position.getVec4(), size.getVec3(1.0), angle),
                Physics(collisionsEnabled, isStatic, bounce, density, friction),
                Visuals(
                    atlasLocation,
                    texture,
                    (TEXTUREMODE)textureMode,
                    textureSize.getVec2(),
                    glm::vec4(color.getVec3() / 255.0f, opacity),
                    opacity),
                activeTags);
        }

        static auto& addDefinition(
            blf::st::ObjectTable& objectTable,
            blf::st::ClassDefinition<BLF_Vector3>* vec3Definition,
            blf::st::ClassDefinition<BLF_Vector2>* vec2Definition,
            blf::st::ClassDefinition<BLF_Color>* colorDefinition) {
            return objectTable.define<BLF_Tile>(
                getName(),
                field("Position", &BLF_Tile::position, *vec3Definition),
                field("Size", &BLF_Tile::size, *vec2Definition),
                field("Angle", &BLF_Tile::angle),
                field("CollisionsEnabled", &BLF_Tile::collisionsEnabled),
                field("Static", &BLF_Tile::isStatic),
                field("Bounce", &BLF_Tile::bounce),
                field("Density", &BLF_Tile::density),
                field("Friction", &BLF_Tile::friction),
                field("Texture", &BLF_Tile::texture),
                field("TextureMode", &BLF_Tile::textureMode),
                field("TextureSize", &BLF_Tile::textureSize, *vec2Definition),
                field("Color", &BLF_Tile::color, *colorDefinition),
                field("Opacity", &BLF_Tile::opacity),
                field("Tags", &BLF_Tile::tags));
        }
};

class BLF_ObjectTableHandler {
    private:
        blf::st::ObjectTable m_objectTable;

    public:
        BLF_ObjectTableHandler();
        blf::st::ObjectTable& getObjectTable();
        blf::st::ClassDefinition<BLF_Vector3>* getVec3Definition() const;
        blf::st::ClassDefinition<BLF_Vector2>* getVec2Definition() const;
        blf::st::ClassDefinition<BLF_Color>* getColorDefinition() const;
        blf::st::ClassDefinition<BLF_Tile>* getTileDefinition() const;
};

class BLF_Converter {
    private:
        static BLF_ObjectTableHandler m_tableHandler;

    public:
        static void load_file(const char* path);
        static void write_file(const char* path);
        static bool load_tag(std::string tag, int index);
};
