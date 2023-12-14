#include "Camera.h"

Camera::Camera()
{
	this->cameraType = PERSPECTIVE;
	this->nearPlane = 0.1f;
	this->farPlane = 100.0f;
	this->fov = 45.0f;

	transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	transform.SetOrientationFromDirections(glm::vec3(0, 1, 0), glm::vec3(0,1,0));
}

void Camera::InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov) 
{
	cameraType = _cameraType;
	cameraWidth = _cameraWidth;
	cameraHeight = _cameraHeight;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
	fov = _fov;

	SetCameraProjection();
}

void Camera::InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane)
{
	cameraType = _cameraType;
	cameraWidth = _cameraWidth;
	cameraHeight = _cameraHeight;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
	fov = 0;

	SetCameraProjection();
}

void Camera::InitializeCamera()
{
	SetCameraProjection();
}

Camera::Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov) :
	cameraType{ _cameraType }, cameraWidth{ _cameraWidth }, cameraHeight{ _cameraHeight }, nearPlane{ _nearPlane }, farPlane{ _farPlane }, fov{ _fov }
{
	SetCameraProjection();
}

Camera::Camera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane) : 
	cameraType{ _cameraType }, cameraWidth{ _cameraWidth }, cameraHeight{ _cameraHeight }, nearPlane{ _nearPlane }, farPlane{ _farPlane }, fov{ 0 }
{
	SetCameraProjection();
}

void Camera::SetCameraProjection()
{
	if (cameraType == PERSPECTIVE)
	{
		cameraMatrix = glm::perspective(glm::radians(fov), cameraWidth / cameraHeight, nearPlane, farPlane);
	}
	else
	{
		cameraMatrix = glm::ortho(0.0f, cameraWidth, 0.0f, cameraHeight, nearPlane, farPlane);
	}
}

void Camera::ChangeCameraType(ECameraType type)
{
	cameraType = type;
	SetCameraProjection();
}

//void Camera::SetCameraPosition(const glm::vec3& pos)
//{
//	transform.SetPosition(pos);
//}
//
//void Camera::SetCameraRotation(const glm::vec3& rot)
//{
//	transform.SetRotation(rot);
//
//	SetCameraForward();
//}

void Camera::SetCameraForward()
{
	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(transform.rotation.y)) * glm::cos(glm::radians(transform.rotation.x));
	direction.y = glm::sin(glm::radians(transform.rotation.x));
	direction.z = glm::sin(glm::radians(transform.rotation.y)) * glm::cos(glm::radians(transform.rotation.x));

	transform.SetForward(glm::normalize(direction));
}

void Camera::SetCameraWidth(const float& width)
{
	this->cameraWidth = width;
}

void Camera::SetCameraHeight(const float& height)
{
	this->cameraHeight = height;
}

Transform* Camera::GetTransform()
{
	return &transform;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	return view;
}
