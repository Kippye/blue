#version 430 core
layout (location = 0) in vec4 aInstanceTransformData; // xPos / yPos / xSize / ySize
layout(location = 1) in vec4 aInstanceTextureData; // texX / texY / texSizeX / texSizeY
layout (location = 2) in vec4 aInstanceColorData; // colorR / colorG / colorB / colorA (opacity * 255)
layout (location = 3) in vec4 aInstanceAdditionalData; // shouldTile / selected / free / isGizmo

/*
desired structure after adding second atlas coords
layout (location = 0) in vec4 aInstanceTransformData; // xPos / yPos / xSize / ySize
layout (location = 1) in vec4 aInstanceTextureData; // texSizeX / texSizeY / free / free
layout (location = 2) in vec4 aInstanceAtlasData; // texX / texY / texX2 / texY2
layout (location = 3) in vec4 aInstanceColorData; // colorR / colorG / colorB / colorA (opacity * 255)
layout (location = 4) in vec4 aInstanceAdditionalData; // shouldTile / selected / free / free
*/

// to geometry shader
out VS_OUT
{
	vec2 pos;
	vec2 size;
	vec2 atlasCoord;
	vec2 texSize;
	vec4 color;
	int shouldTile;
	int selected;
	mat4 matrix;
} vs_out;

uniform mat4 view; // world to view
uniform mat4 projection; // view to projection

void main()
{
	vs_out.pos = aInstanceTransformData.xy;
	vs_out.size = aInstanceTransformData.zw;
	vs_out.atlasCoord = aInstanceTextureData.xy;
	vs_out.texSize = aInstanceTextureData.zw;
	vs_out.color = aInstanceColorData.xyzw;
	vs_out.shouldTile = int(aInstanceAdditionalData.x);
	vs_out.selected = int(aInstanceAdditionalData.y);
	vs_out.matrix = (projection * view);
};
