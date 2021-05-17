#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <main.h>
#include <render.h>

class Program;
extern Program program;

void Render::setup()
{
	// [debug] glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//~ // general settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	// position attribute
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

void Render::test()
{
	// TEMP: loading 10000 tiles in a 100 by 100 square with different textures //
	int x = 0, y = 0;
	int texX = 0, texY = 0;
    for (int i = 0; i < INSTANCE_CAP / 2; i++)
    {
		if (texX ==	textureAtlas->width / 16)
		{
			texX = 0;
			y++;
			texY ++;
		}
		if (texY == textureAtlas->height / 16)
		{
			texX = 0;
			texY = 0;
		}

		glm::vec4 instance;
		instance.x = x;
		instance.y = y;
		instance.z = texX;
		instance.w = texY;
		instanceData[i] = instance;
		x ++;
		texX ++;
	}

	glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * INSTANCE_CAP, &instanceData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::render()
{
	// render loop
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	timeCounter += deltaTime;

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

	// set uniforms
	glm::mat4 model = glm::mat4(1.0f);
	// update the camera view direction (not really needed but eh)
	program.camera.updateView();
	// projection  				   					FOV													ASPECT RATIO									CLIP RANGE
	glm::mat4 projection = glm::perspective(glm::radians(program.camera.FOV), (float)(program.windowManager.SCREEN_WIDTH / program.windowManager.SCREEN_HEIGHT), 0.1f, 100.0f);
	/* glm::mat4 projection = glm::ortho
	(
		0.0f, program.windowManager.SCREEN_WIDTH, 0.0f, program.windowManager.SCREEN_HEIGHT,
		-1000.0f, 1000.0f
	);*/

	shader.setMat4("model", model);
	shader.setMat4("view", program.camera.view);
	shader.setMat4("projection", projection);
	//~glm::mat4 model = glm::translate(glm::mat4(1.0f), program.levelView.currentTiles[tile].transforms.Position);

	if (textureAtlas != nullptr)
	shader.setVec2("atlasSize", glm::vec2(textureAtlas->width, textureAtlas->height));

	// use the texture atlas
	if (textureAtlas != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlas->ID);
		glBindVertexArray(VAO);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, program.levelView.currentTiles.size());
	}
	program.gui.drawGui();

	glfwSwapBuffers(program.windowManager.window);
}

void Render::terminate()
{
	// dead
    glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
