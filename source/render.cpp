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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// every shader and render call will now use this shader program
	shader = Shader("shaders/shader_vert.glsl", "shaders/shader_geom.glsl", "shaders/shader_frag.glsl");

	shader.use();
	shader.setInt("texture1", 0);
}

void Render::updateInstanceArray(INSTANCE_ARRAY_UPDATE type)
{
	glBindVertexArray(VAO);

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceTransformData.size(), instanceTransformData.data(), GL_STATIC_DRAW);

		// also set instance data
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(0, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_2)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceAdditionalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceAdditionalData.size(), instanceAdditionalData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
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
		glDrawElementsInstanced(GL_POINTS, 1, GL_UNSIGNED_INT, 0, program.editor.tiles.size());
	}

	program.gui.drawGui();

	glfwSwapBuffers(program.windowManager.window);
}

void Render::set_tile_selection(int index, bool to)
{
	instanceAdditionalData[index].w = float(to);
	updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);
}

void Render::set_tile_selection(std::vector<int> &indices, bool to)
{
	for (int i = 0; i < indices.size(); i++)
	{
		instanceAdditionalData[indices[i]].w = float(to);
	}

	updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);
}

void Render::add_to_instance_data(E_Tile &tile)
{
	instanceTransformData.emplace_back(tile.location.Position.x, tile.location.Position.y, tile.location.Size.x, tile.location.Size.y);
	instanceAdditionalData.emplace_back(tile.visuals.atlasCoords.x, tile.visuals.atlasCoords.y, tile.visuals.TextureMode == TEXTUREMODE_TILE, tile.selected);
}

void Render::erase_from_instance_data(int index)
{
	instanceTransformData.erase(instanceTransformData.begin() + index);
	instanceAdditionalData.erase(instanceAdditionalData.begin() + index);
}

void Render::add_to_render_list(E_Tile &tile)
{
	add_to_instance_data(tile);
	updateInstanceArray();
}

void Render::add_to_render_list(std::vector<E_Tile> &tiles)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		add_to_instance_data(tiles[i]);
	}

	updateInstanceArray();
}

// TODO: more functions with fucking pointer vectors cus this shit sucky af

void Render::remove_from_render_list(int index)
{
	erase_from_instance_data(index);
	updateInstanceArray();
}

void Render::remove_from_render_list(std::vector<int> &indices)
{
	for (int i = indices.size() - 1; i >= 0; i--)
	{
		std::cout << "index: " << indices[i] << std::endl;
		erase_from_instance_data(indices[i]);
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
