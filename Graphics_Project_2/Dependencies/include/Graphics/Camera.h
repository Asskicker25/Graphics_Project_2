#pragma once

#include "Debugger.h"

enum ECameraType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera
{
private:
	ECameraType cameraType;
	float cameraWidth;
	float cameraHeight;
	float nearPlane;
	float farPlane;
	float fov;

	glm::mat4 cameraMatrix;

	void SetCameraProjection();

public:
	Camera();
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void InitializeCamera();
	void ChangeCameraType(ECameraType type);

	void SetCameraPosition(const glm::vec3& pos);
	void SetCameraRotation(const glm::vec3& rot);

	void SetCameraForward();

	void SetCameraWidth(const float& width);
	void SetCameraHeight(const float& height);

	inline glm::mat4 GetMatrix() const { return cameraMatrix; }

	float cameraPitch{ 0 };
	float cameraYaw{ -90.0f };

	glm::vec3 cameraPos{ 0.0f,0.0f,3.0f };
	glm::vec3 cameraFront{ 0.0f,0.0f,-1.0f };
	glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

};

