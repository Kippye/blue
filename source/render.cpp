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
	glGenBuffers(1, &instanceTextureVBO);
	glGenBuffers(1, &instanceAtlasVBO);
	glGenBuffers(1, &instanceColorVBO);
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


	/// GIZMOS
	glGenBuffers(1, &instanceVBO_G);
	glGenBuffers(1, &instanceTextureVBO_G);
	glGenBuffers(1, &instanceAtlasVBO_G);
	glGenBuffers(1, &instanceColorVBO_G);
	glGenBuffers(1, &instanceAdditionalVBO_G);

	glGenVertexArrays(1, &VAO_G);
	glGenBuffers(1, &VBO_G);
	glGenBuffers(1, &EBO_G);

	glBindVertexArray(VAO_G);
	// copy array of vertices to buffer for OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_G);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// every shader and render call will now use this shader program
	shader = Shader("shaders/tile.vert", "shaders/tile.geom", "shaders/tile.frag");

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
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_2)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceTextureVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceTextureData.size(), instanceTextureData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_3)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceAtlasVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::uvec4) * instanceAtlasData.size(), instanceAtlasData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_INT, GL_FALSE, 4 * sizeof(unsigned int), (void*)0);
		glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_4)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceColorData.size(), instanceColorData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_5)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceAdditionalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceAdditionalData.size(), instanceAdditionalData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}
}

void Render::updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE type)
{
	glBindVertexArray(VAO_G);

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_G);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * GinstanceTransformData.size(), GinstanceTransformData.data(), GL_STATIC_DRAW);

		// also set instance data
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(0, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_2)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceTextureVBO_G);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * GinstanceTextureData.size(), GinstanceTextureData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_3)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceAtlasVBO_G);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::uvec4) * GinstanceAtlasData.size(), GinstanceAtlasData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_INT, GL_FALSE, 4 * sizeof(unsigned int), (void*)0);
		glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_3)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO_G);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * GinstanceColorData.size(), GinstanceColorData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
	}

	if (type == INSTANCE_ARRAY_UPDATE_ALL || type & INSTANCE_ARRAY_UPDATE_4)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceAdditionalVBO_G);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * GinstanceAdditionalData.size(), GinstanceAdditionalData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1); // tell OpenGL this is an instanced vertex attribute.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		instanceDataUpdates++;
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

	if (mouse_button_delay >= 0.05f && mouse_button_delay < 100.0f)
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
	glClearColor(program.editor.backgroundColor.r, program.editor.backgroundColor.g, program.editor.backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the general purpose shader
	shader.use();

	// set uniforms
	shader.setMat4("view", program.camera.view);
	shader.setMat4("projection", program.camera.projection);

	if (textureAtlas != nullptr)
	{
		shader.setVec2("inTexAtlasSize", glm::vec2(textureAtlas->width, textureAtlas->height));

		// use the texture atlas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlas->ID);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_POINTS, 1, GL_UNSIGNED_INT, 0, program.editor.tiles.size());
	}

	glBindVertexArray(VAO_G);
	glDrawElementsInstanced(GL_POINTS, 1, GL_UNSIGNED_INT, 0, program.editor.gizmos.size());

	program.gui.drawGui();

	glfwSwapBuffers(program.windowManager.window);
}

void Render::set_tile_selection(int index, bool to)
{
	instanceAdditionalData[index].y = float(to);
	updateInstanceArray(INSTANCE_ARRAY_UPDATE_5);
}

void Render::set_tile_selection(std::vector<int> &indices, bool to)
{
	for (size_t i = 0; i < indices.size(); i++)
	{
		instanceAdditionalData[indices[i]].y = float(to);
	}

	updateInstanceArray(INSTANCE_ARRAY_UPDATE_5);
}

void Render::add_to_instance_data(E_Tile &tile)
{
	instanceTransformData.emplace_back(tile.location.Position.x, tile.location.Position.y, tile.location.Size.x, tile.location.Size.y);
	instanceTextureData.emplace_back(tile.visuals.TextureSize.x, tile.visuals.TextureSize.y, 0.0f, 0.0f);
	instanceAtlasData.emplace_back(tile.visuals.atlasLocation.x, tile.visuals.atlasLocation.y, tile.visuals.atlasLocation.z, tile.visuals.atlasLocation.w);
	instanceColorData.emplace_back(tile.visuals.Color.x, tile.visuals.Color.y, tile.visuals.Color.z, tile.visuals.Opacity);
	instanceAdditionalData.emplace_back(tile.visuals.TextureMode == TEXTUREMODE_TILE, tile.selected, 0.0f, 0.0f);
	instanceDataUpdates++;
}

void Render::add_gizmo_to_instance_data(Gizmo &gizmo)
{
	GinstanceTransformData.emplace_back(gizmo.location.Position.x, gizmo.location.Position.y, gizmo.location.Size.x, gizmo.location.Size.y);
	GinstanceTextureData.emplace_back(gizmo.visuals.TextureSize.x, gizmo.visuals.TextureSize.y, 0.0f, 0.0f);
	GinstanceAtlasData.emplace_back(gizmo.visuals.atlasLocation.x, gizmo.visuals.atlasLocation.y, gizmo.visuals.atlasLocation.z, gizmo.visuals.atlasLocation.w);
	GinstanceColorData.emplace_back(gizmo.visuals.Color.x, gizmo.visuals.Color.y, gizmo.visuals.Color.z, gizmo.visuals.Opacity);
	GinstanceAdditionalData.emplace_back(gizmo.visuals.TextureMode == TEXTUREMODE_TILE, 0.0f, 0.0f, 1.0f);
	instanceDataUpdates++;
}

void Render::erase_from_instance_data(int index)
{
	instanceTransformData.erase(instanceTransformData.begin() + index);
	instanceTextureData.erase(instanceTextureData.begin() + index);
	instanceAtlasData.erase(instanceAtlasData.begin() + index);
	instanceColorData.erase(instanceColorData.begin() + index);
	instanceAdditionalData.erase(instanceAdditionalData.begin() + index);
	instanceDataUpdates++;
}

void Render::erase_gizmo_from_instance_data(int index)
{
	GinstanceTransformData.erase(GinstanceTransformData.begin() + index);
	GinstanceTextureData.erase(GinstanceTextureData.begin() + index);
	GinstanceAtlasData.erase(GinstanceAtlasData.begin() + index);
	GinstanceColorData.erase(GinstanceColorData.begin() + index);
	GinstanceAdditionalData.erase(GinstanceAdditionalData.begin() + index);
	instanceDataUpdates++;
}

void Render::add_to_render_list(E_Tile &tile)
{
	add_to_instance_data(tile);
	updateInstanceArray();
}

void Render::add_to_render_list(std::vector<E_Tile> &tiles)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		add_to_instance_data(tiles[i]);
	}

	updateInstanceArray();
}

void Render::add_gizmo_to_render_list(Gizmo &gizmo)
{
	add_gizmo_to_instance_data(gizmo);
	updateGizmoInstanceArray();
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
		erase_from_instance_data(indices[i]);
	}

	updateInstanceArray();
}

void Render::remove_gizmo_from_render_list(int index)
{
	erase_gizmo_from_instance_data(index);
	updateGizmoInstanceArray();
}

void Render::terminate()
{
	// dead
    glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &instanceTextureVBO);
    glDeleteBuffers(1, &instanceAtlasVBO);
    glDeleteBuffers(1, &instanceColorVBO);
    glDeleteBuffers(1, &instanceAdditionalVBO);
	glDeleteBuffers(1, &EBO_G);
	glDeleteVertexArrays(1, &VAO_G);
    glDeleteBuffers(1, &VBO_G);
    glDeleteBuffers(1, &instanceVBO_G);
    glDeleteBuffers(1, &instanceTextureVBO_G);
    glDeleteBuffers(1, &instanceAtlasVBO_G);
    glDeleteBuffers(1, &instanceColorVBO_G);
    glDeleteBuffers(1, &instanceAdditionalVBO_G);
}
