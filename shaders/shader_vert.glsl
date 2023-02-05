#version 430 core
layout (location = 0) in vec4 aInstanceTransformData; // xPos / yPos / xSize / ySize
layout (location = 1) in vec4 aInstanceAdditionalData; // texX / texY / shouldTile / selected
layout (location = 2) in vec4 aInstanceColorData; // colorR / colorG / colorB / colorA (opacity * 255)

// to geometry shader
out VS_OUT
{
	vec2 pos;
	vec2 size;
	vec2 atlasCoord;
	int shouldTile;
	int selected;
	vec4 color;
	mat4 matrix;
} vs_out;

uniform mat4 view; // world to view
uniform mat4 projection; // view to projection

void main()
{
	vs_out.pos = aInstanceTransformData.xy;
	vs_out.size = aInstanceTransformData.zw;
	vs_out.atlasCoord = aInstanceAdditionalData.xy;
	vs_out.shouldTile = int(aInstanceAdditionalData.z);
	vs_out.selected = int(aInstanceAdditionalData.w);
	vs_out.color = aInstanceColorData.xyzw;
	vs_out.matrix = (projection * view);
};
