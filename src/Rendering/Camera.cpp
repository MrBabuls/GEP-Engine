#include "Camera.h"

Camera::Camera(glm::vec2 resolution, glm::vec2 frustum) :
	currentResolution(resolution),
	currentFrustum(frustum) {
}

auto Camera::GetProjectionMatrix() const -> glm::mat4
{
	return glm::ortho(
		currentResolution.x * -0.5f,
		currentResolution.x * 0.5f,
		currentResolution.y * -0.5f,
		currentResolution.y * 0.5f,
		currentFrustum.x,
		currentFrustum.y
	);
}

auto Camera::GetViewMatrix() const -> glm::mat4
{
	return glm::lookAt(
		transform.Position,
		transform.Position + glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f, 1.f, 0.f)
	);
}