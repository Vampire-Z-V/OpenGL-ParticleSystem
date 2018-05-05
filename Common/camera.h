#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

namespace default_value {
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 3.0f;
	const float SENSITIVTY = 0.1f;
	const float ZOOM = 45.0f;

	const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
}


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = default_value::CAMERA_POSITION,
		glm::vec3 up = default_value::WORLD_UP,
		float yaw = default_value::YAW, float pitch = default_value::PITCH);
	Camera(
		float posX, float posY, float posZ, 
		float upX, float upY, float upZ, 
		float yaw, float pitch
	);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);


private:
	void updateCameraVectors();

};
#endif