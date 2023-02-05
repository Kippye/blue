#version 430 core

out vec4 FragColor;

in vec2 TexCoord;
flat in vec2 bottomLeftTexCoord;
flat in vec2 atlasCoord;
flat in vec2 tileSize;
flat in vec2 texAtlasSize;
flat in int shouldTile;
flat in int selected;

uniform sampler2D texture1;

void main()
{
	vec2 finalCoords = TexCoord;
	vec4 selectionColor = vec4(selected / 4.0f);

	if (shouldTile == 1)
	{
		vec2 atlasTextureSize = vec2(1.0, 1.0) / texAtlasSize;

		// didn't have a better name
		// use before texcoord is modified, we need the original one from geometry shader.
		// we need it because we're able to locate this pixel's exact location inside of the tile
		vec2 sizeTexCoord = TexCoord * tileSize;

		// we can finally modulo it lol
		// i dont recall if modulos work on vectors but if they don't you're just gonna have to make a vec2
		// by using modulo on both x and y.
		vec2 tiledTexCoord = mod(sizeTexCoord, atlasTextureSize);

		// next order of business: map the tiledTexCoord in range of the atlas texture coordinates.
		// this is going to be simple, since the tiledTexCoord is a coordinate whose maximum size will be 0-1
		// so let's constrain that further.
		vec2 constrainedCoords = tiledTexCoord * atlasTextureSize;
		// the coords are now 0 - well, whatever your texture size is.

		// we have begun 'mapping' the coords onto the atlas.
		// last thing to do is add the texture coordinates to the bottom left of the texture in the atlas, if everything went to plan.
		// so just rename whatever that was to be separate from texcoord
		finalCoords = bottomLeftTexCoord + tiledTexCoord - (atlasTextureSize * vec2(0, 1));
	}

	FragColor = texture(texture1, finalCoords) + selectionColor;
};
