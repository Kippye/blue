#version 430 core
layout (triangle_strip, max_vertices = 4) out;
// to frag shader
out vec2 TexCoord;
flat out vec2 bottomLeftTexCoord;
flat out vec2 tileSize;
flat out vec2 textureSize;
flat out vec2 atlasCoord;
flat out vec2 texPixelSize;
flat out vec2 atlasSize;
flat out vec4 color;
flat out int shouldTile;
flat out int selected;
// from vec shader
layout (points) in;
in VS_OUT
{
	vec2 pos;
	vec2 size;
	vec2 texSize;
	vec2 atlasCoord;
	vec2 texPixelSize;
	vec4 color;
	int shouldTile;
	int selected;
	mat4 matrix;
} gs_in[];

uniform vec2 inTexAtlasSize;

// x = atlasCoord.X / atlasSize.X + texPixelSize.X / atlasSize.X

vec2 getTexCoord(vec2 atlasPos)
{
	return gs_in[0].atlasCoord / inTexAtlasSize + (gs_in[0].texPixelSize / inTexAtlasSize * vec2(atlasPos.x, 1.0f - atlasPos.y));
}

// vec2 getTexCoord(vec2 atlasPos)
// {
// 	return vec2(((atlasPos.x / atlasSize.x) * 16) + (gs_in[0].atlasCoord.x / atlasSize.x) * 16,
// 	1 - (((atlasPos.y / atlasSize.y) * 16) + (((atlasSize.y / 16 - 1) - gs_in[0].atlasCoord.y) / atlasSize.y) * 16));
// }

vec2 getBottomLeftTexCoord()
{
	return gs_in[0].atlasCoord / inTexAtlasSize;
}

// vec2 getBottomLeftTexCoord()
// {
// 	return vec2(((0 / atlasSize.x) * 16) + (gs_in[0].atlasCoord.x / atlasSize.x) * 16,
// 	1 - (((0 / atlasSize.y) * 16) + (((atlasSize.y / 16 - 1) - gs_in[0].atlasCoord.y) / atlasSize.y) * 16));
// }

void main() {
	// BOTTOM LEFT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x, gs_in[0].pos.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(0.0f, 0.0f));
		tileSize = gs_in[0].size;
		textureSize = gs_in[0].texSize;
		atlasCoord = gs_in[0].atlasCoord;
		texPixelSize = gs_in[0].texPixelSize;
		atlasSize = inTexAtlasSize;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
    EmitVertex();
	// BOTTOM RIGHT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x + gs_in[0].size.x, gs_in[0].pos.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(1.0f, 0.0f));
		tileSize = gs_in[0].size;
		textureSize = gs_in[0].texSize;
		atlasCoord = gs_in[0].atlasCoord;
		texPixelSize = gs_in[0].texPixelSize;
		atlasSize = inTexAtlasSize;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();
	// TOP LEFT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x, gs_in[0].pos.y + gs_in[0].size.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(0.0f, 1.0f));
		tileSize = gs_in[0].size;
		textureSize = gs_in[0].texSize;
		atlasCoord = gs_in[0].atlasCoord;
		texPixelSize = gs_in[0].texPixelSize;
		atlasSize = inTexAtlasSize;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();
	// TOP RIGHT
	gl_Position = gs_in[0].matrix * vec4(gs_in[0].pos.x + gs_in[0].size.x, gs_in[0].pos.y + gs_in[0].size.y, 0.0f, 1.0f);
		TexCoord = getTexCoord(vec2(1.0f, 1.0f));
		tileSize = gs_in[0].size;
		textureSize = gs_in[0].texSize;
		atlasCoord = gs_in[0].atlasCoord;
		texPixelSize = gs_in[0].texPixelSize;
		atlasSize = inTexAtlasSize;
		shouldTile = gs_in[0].shouldTile;
		selected = gs_in[0].selected;
		color = gs_in[0].color;
		bottomLeftTexCoord = getBottomLeftTexCoord();
	EmitVertex();

    EndPrimitive();
}