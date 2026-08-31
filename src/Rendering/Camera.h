#pragma once
#include "..\Core\Transform.h"

class Camera
{
public:
	Camera(glm::vec2 resolution, glm::vec2 frustum);
	auto GetProjectionMatrix() const -> glm::mat4;
	auto GetViewMatrix() const -> glm::mat4;
	inline auto* GetTransform() { return &transform; }
	inline auto* GetTransform() const { return &transform; }
	inline auto SetResolution(glm::vec2 resolution) { currentResolution = resolution; }

	static Camera DefaultCamera()
	{
		Camera camera(glm::vec2(640.0f, 480.0f), glm::vec2(-100.0f, 100.0f));
		camera.transform.Position = glm::vec3(0.f, 0.f, -1.f);
		return camera;
	}

private:
	Transform transform;
	glm::vec2 currentResolution{ 0.f };
	glm::vec2 currentFrustum{ 0.f };
};