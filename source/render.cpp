#include <GLFW/glfw3.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <main.h>
#include <render.h>

class Program;
extern Program program;

void Render::setup()
{
	// [debug] glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//~ // general settings
	//glEnable(GL_DEPTH_TEST);
	// add blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenBuffers(1, &instanceVBO);
	glGenBuffers(1, &instanceAdditionalVBO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// copy array of vertices to buffer for OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set vertex attribute pointers
	// this is the "stride" value, aka sum of position, normal and texcoords
	int stride = 5;
	// position attribute	std::cout << glGetError() << std::endl;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// every shader and render call will now use this shader program
	shader = Shader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl");

	shader.use();
	shader.setInt("texture1", 0);
}

void Render::updateInstanceArray()
{
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceTransformData.size(), instanceTransformData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, instanceAdditionalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceAdditionalData.size(), instanceAdditionalData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, instanceAdditionalVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::render()
{
	// render loop
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	timeCounter += deltaTime;
	mouse_button_delay += deltaTime;

	if (mouse_button_delay >= 0.1f && mouse_button_delay < 100.0f)
	{
		mouse_repeat = true;
		mouse_button_delay = 1000.0f;
	}
	else
	{
		mouse_repeat = false;
	}
	if (timeCounter >= 0.25f)
	{
		FPS = (int)((1.0f / deltaTime) + 0.5f);
		timeCounter = 0.0f;
	}

	//glClearColor(0.5f, 0.2f, 1.0f, 1.0f); // cartoony sky
	//glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // dark gray color
	glClearColor(0.2, 0.2, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the general purpose shader
	shader.use();

	// TODO? orthographic projection
	/* glm::mat4 projection = glm::ortho
	(
		0.0f, program.windowManager.SCREEN_WIDTH, 0.0f, program.windowManager.SCREEN_HEIGHT,
		-1000.0f, 1000.0f	std::cout << glGetError() << std::endl;

	);*/
	// set uniforms

	shader.setMat4("view", program.camera.view);
	shader.setMat4("projection", program.camera.projection);

	if (textureAtlas != nullptr)
	{
		shader.setVec2("atlasSize", glm::vec2(textureAtlas->width, textureAtlas->height));

		// use the texture atlas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlas->ID);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, program.editor.tiles.size());
	}

	program.gui.drawGui();

	glfwSwapBuffers(program.windowManager.window);
}

void Render::add_to_render_list(E_Tile &tile)
{
	//instanceData.emplace_back(glm::vec4(tile.location.Position.x, tile.location.Position.y, tile.visuals.atlasCoords.x, tile.visuals.atlasCoords.y));
	instanceTransformData.emplace_back(tile.location.Position.x, tile.location.Position.y, tile.location.Size.x, tile.location.Size.y);
	instanceAdditionalData.emplace_back(tile.visuals.atlasCoords.x, tile.visuals.atlasCoords.y, 0.0f, 0.0f);

	updateInstanceArray();
}

void Render::add_to_render_list(std::vector<E_Tile> &tiles)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		instanceTransformData.emplace_back(tiles[i].location.Position.x, tiles[i].location.Position.y, tiles[i].location.Size.x, tiles[i].location.Size.y);
		instanceAdditionalData.emplace_back(tiles[i].visuals.atlasCoords.x, tiles[i].visuals.atlasCoords.y, 0.0f, 0.0f);
	}

	updateInstanceArray();
}

void Render::remove_from_render_list(E_Tile &tile, int index)
{
	instanceTransformData.erase(instanceTransformData.begin() + index);
	instanceAdditionalData.erase(instanceAdditionalData.begin() + index);
	updateInstanceArray();
}

void Render::remove_from_render_list(std::vector<E_Tile> &tiles, std::vector<int> &indices)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		instanceTransformData.erase(instanceTransformData.begin() + indices[i]);
		instanceAdditionalData.erase(instanceAdditionalData.begin() + indices[i]);
	}

	updateInstanceArray();
}

void Render::terminate()
{
	// dead
    glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &instanceAdditionalVBO);
}
