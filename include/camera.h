#pragma once

#include <algorithm>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
	public:
		static const inline float CAMERA_SPEED = 14.0f;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 movementFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view;
		float yaw = -90.0f, pitch = 0.0f;
		float FOV = 45.0f;
		float zoomSpeed = 1.5f;
		float currentCameraSpeed = CAMERA_SPEED;
		const float cameraSensitivity = 0.1f;

	float yVelocity = 0.0f;

	public:
	Camera()
	{
		// create a lookat matrix
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	public:
	void moveCamera(glm::vec3 movement)
	{
		cameraPos += movement * currentCameraSpeed;
		cameraPos.z = std::clamp(cameraPos.z, 2.0f, 30.0f);
	}

	public:
	void updateView()
	{
		// maybe this should be in moveCamera, if i do make mouse looking as well
		pitch = std::clamp(pitch, -89.0f, 89.0f);

		// calculate new front vector
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
		direction.y = 0;
		movementFront = glm::normalize(direction);

		cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
};
