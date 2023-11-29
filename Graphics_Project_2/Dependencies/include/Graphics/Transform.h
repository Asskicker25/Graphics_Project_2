#pragma once

#include "Debugger.h"

class Transform
{

public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::quat quaternionRotation;
	glm::vec3 scale;

	Transform();
	Transform(const Transform& transform);

	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rotation);
	void SetQuatRotation(glm::quat quatRotation);
	void SetScale(glm::vec3 scale);

	glm::mat4 GetTransformMatrix();

	glm::mat4 GetInverseMatrix();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();

	void SetUp(glm::vec3 newUp);
	void SetRight(glm::vec3 newRight);
	void SetForward(glm::vec3 newForward);
	void SetOrientationFromDirections(glm::vec3 newUp, glm::vec3 newRight);

private:
	void UpdateQuaternionFromEuler();
	void UpdateEulerFromQuaternion();
};

