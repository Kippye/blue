#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // texture coordinate
layout (location = 2) in vec4 aInstanceTransformData; // xPos, yPos, xSize, ySize
layout (location = 3) in vec4 aInstanceAdditionalData; // texture atlas indices

out vec2 TexCoord;

uniform mat4 view; // world to view
uniform mat4 projection; // view to projection

uniform vec2 atlasSize;

void main()
{
	// create the screen image, multiplication is right to left
	//gl_Position = projection * view * vec4(aPos, 1.0f); // apply our transform matrices
	// TODO: create a model matrix and transform it
	gl_Position = projection * view * vec4(aPos.xy + aInstanceTransformData.xy, aPos.z, 1.0f); // apply our transform matrices
	//TexCoord = aTexCoord;
	TexCoord = vec2(((aTexCoord.x / atlasSize.x) * 16) + (aInstanceAdditionalData.x / atlasSize.x) * 16,
	1 - (((aTexCoord.y / atlasSize.y) * 16) + (aInstanceAdditionalData.y / atlasSize.y) * 16));
};
