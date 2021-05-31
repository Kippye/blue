#version 430 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
	// gl_FragCoord -- gets the SCREEN position of this fragment, pretty damn useful!

	vec4 texColor = texture(texture1, TexCoord);

	FragColor = texColor;
};
