#pragma once

#include "Debugger.h"
#include "Transform.h"

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
	Transform transform;

	Camera();
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void InitializeCamera();
	void ChangeCameraType(ECameraType type);

	//void SetCameraPosition(const glm::vec3& pos);
	//void SetCameraRotation(const glm::vec3& rot);

	void SetCameraForward();

	void SetCameraWidth(const float& width);
	void SetCameraHeight(const float& height);

	Transform* GetTransform();

	glm::mat4 GetViewMatrix();

	inline glm::mat4 GetMatrix() const { return cameraMatrix; }

};

