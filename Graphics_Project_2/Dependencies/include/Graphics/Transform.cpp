#include "Transform.h"

Transform::Transform() : position{ glm::vec3(0) }, rotation{ glm::vec3(0) }, scale{ glm::vec3(1.0f) }
{
}

Transform::Transform(const Transform& transform)
{
	position = transform.position;
	rotation = transform.rotation;
	scale = transform.scale;
}

void Transform::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void Transform::SetRotation(glm::vec3 _rotation)
{
	rotation = _rotation;
	UpdateQuaternionFromEuler();
}

void Transform::SetQuatRotation(glm::quat quatRotation)
{
	quaternionRotation = quatRotation;
	UpdateEulerFromQuaternion();
}

void Transform::SetScale(glm::vec3 _scale)
{
	scale = _scale;
}


glm::mat4 Transform::GetTransformMatrix()
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position);

	/*trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0, 0, 1));*/

	trans = trans * glm::mat4_cast(quaternionRotation);

	trans = glm::scale(trans, scale);

	return trans;
}

glm::mat4 Transform::GetInverseMatrix()
{
	return glm::inverse(glm::transpose(GetTransformMatrix()));
}

glm::vec3 Transform::GetForward()
{
	return glm::normalize(-glm::vec3(glm::mat4_cast(quaternionRotation)[2]));
	//return glm::normalize(-glm::vec3(GetTransformMatrix()[2]));
}

glm::vec3 Transform::GetUp()
{
	return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[1]));
	//return glm::normalize(glm::vec3(GetTransformMatrix()[1]));
}

glm::vec3 Transform::GetRight()
{
	return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[0]));
	//return glm::normalize(glm::vec3(GetTransformMatrix()[0]));
}


void Transform::SetUp(glm::vec3 newUp)
{
	newUp = glm::normalize(newUp);

	glm::vec3 axis = glm::cross(this->GetUp(), newUp);
	float angle = glm::acos(glm::dot(this->GetUp(), newUp));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetRight(glm::vec3 newRight)
{
	newRight = glm::normalize(newRight);

	glm::vec3 axis = glm::cross(this->GetRight(), newRight);
	float angle = glm::acos(glm::dot(this->GetRight(), newRight));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetForward(glm::vec3 newForward)
{
	newForward = glm::normalize(newForward);

	glm::vec3 axis = glm::cross(this->GetForward(), newForward);
	float angle = glm::acos(glm::dot(this->GetForward(), newForward));
	glm::quat rotationQuat = glm::angleAxis(angle, axis);

	this->SetQuatRotation(rotationQuat);
}

void Transform::SetOrientationFromDirections(glm::vec3 newUp, glm::vec3 newRight)
{
	newUp = glm::normalize(newUp);
	newRight = glm::normalize(newRight);
	glm::vec3 newForward = glm::cross(newRight, newUp);

	glm::quat rotationQuat = glm::quatLookAt(newForward, newUp);

	this->SetQuatRotation(rotationQuat);
}


void Transform::UpdateQuaternionFromEuler()
{
	quaternionRotation = glm::quat(glm::radians(rotation));
}

void Transform::UpdateEulerFromQuaternion()
{
	rotation = glm::degrees(glm::eulerAngles(quaternionRotation));
}
