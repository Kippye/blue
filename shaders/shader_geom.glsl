#version 430 core
layout (triangle_strip, max_vertices = 4) out;
// to frag shader
out vec2 TexCoord;
flat out vec2 bottomLeftTexCoord;
flat out vec2 atlasCoord;
flat out vec2 tileSize;
flat out vec2 texAtlasSize;
flat out int shouldTile;
flat out int selected;
flat out vec4 color;
// from vec shader
layout (points) in;
in VS_OUT
{
	vec2 pos;
	vec2 size;
	vec2 atlasCoord;
	vec2 texSize;
	vec4 color;
	int shouldTile;
	int selected;
	mat4 matrix;
} gs_in[];

uniform vec2 atlasSize;

vec2 getTexCoord(vec2 atlasPos)
{
	return vec2(((atlasPos.x / atlasSize.x) * 16) + (gs_in[0].atlasCoord.x / atlasSize.x) * 16,
	1 - (((atlasPos.y / atlasSize.y) * 16) + (((atlasSize.y / 16 - 1) - gs_in[0].atlasCoord.y) / atlasSize.y) * 16));
}

vec2 getBottomLeftTexCoord()
{
	return vec2(((0 / atlasSize.x) * 16) + (gs_in[0].atlasCoord.x / atlasSize.x) * 16,
	1 - (((0 / atlasSize.y) * 16) + (((atlasSize.y / 16 - 1) - gs_in[0].atlasCoord.y) / atlasSize.y) * 16));
}

void main() {
	// BOTTOM LEFT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x, gs_in[0].pos.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(0.0f, 0.0f));
		tileSize = gs_in[0].size;
		texAtlasSize = atlasSize / 16.0f;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
    EmitVertex();
	// BOTTOM RIGHT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x + gs_in[0].size.x, gs_in[0].pos.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(1.0f, 0.0f));
		tileSize = gs_in[0].size;
		texAtlasSize = atlasSize / 16.0f;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();
	// TOP LEFT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x, gs_in[0].pos.y + gs_in[0].size.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(0.0f, 1.0f));
		tileSize = gs_in[0].size;
		texAtlasSize = atlasSize / 16.0f;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();
	// TOP RIGHT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x + gs_in[0].size.x, gs_in[0].pos.y + gs_in[0].size.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(1.0f, 1.0f));
		tileSize = gs_in[0].size;
		texAtlasSize = atlasSize / 16.0f;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();

    EndPrimitive();
}
