#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // texture coordinate
layout (location = 2) in vec4 aInstanceData; // texture atlas indices

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view; // world to view
uniform mat4 projection; // view to projection

uniform vec2 atlasSize;

void main()
{
	// create the screen image, multiplication is right to left
	gl_Position = projection * view * model * vec4(aPos.xy + aInstanceData.xy, 0.0f,  1.0f); // apply our transform matrices
	//TexCoord = /*aTexCoord;*/ vec2((aTexCoord.x / (atlasSize.x / 16)) + (aInstanceData.z / (atlasSize.x / 16)), aTexCoord.y + (aInstanceData.w / (atlasSize.y / 16)));
	TexCoord = vec2(((aTexCoord.x / atlasSize.x) * 16) + (aInstanceData.z / atlasSize.x) * 16,
	1 - (((aTexCoord.y / atlasSize.y) * 16) + (aInstanceData.w / atlasSize.y) * 16));
};
