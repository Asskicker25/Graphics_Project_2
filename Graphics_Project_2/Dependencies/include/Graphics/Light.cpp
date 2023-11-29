#include "Light.h"

Light::Light(Model& model, LightType newLightType) : lightType{ newLightType }, transform {nullptr}, lightModel{ nullptr }
{
	intensity = 0.5f;
	attenuation = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	innerAngle = 30;
	outerAngle = 50;
	InitializeLight(model, newLightType);
}

Light::Light(Model* model, LightType newLightType) : lightType { newLightType }, transform{ nullptr }, lightModel{ nullptr }
{
	intensity = 0.5f;
	attenuation = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	innerAngle = 30;
	outerAngle = 50;
	InitializeLight(model, newLightType);
}

Light::Light() : lightType { Point }, transform { nullptr }, lightModel { nullptr }
{
	intensity = 0.5f;
	attenuation = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	innerAngle = 30;
	outerAngle = 50;
}

Light::~Light()
{
}

void Light::InitializeLight(Model& model, LightType lightType)
{
	this->lightType = lightType;
	lightModel = &model;
	transform = &(lightModel->transform);
}

void Light::InitializeLight(Model* model, LightType lightType)
{
	this->lightType = lightType;
	lightModel = model;
	transform = &(lightModel->transform);
}

glm::vec4 Light::GetLightColor()
{ 
	glm::vec4 baseColor = glm::vec4(
		material.GetBaseColor().x * intensity, 
		material.GetBaseColor().y * intensity,
		material.GetBaseColor().z * intensity,
		material.GetBaseColor().w);

	return baseColor;
}

glm::vec4 Light::GetAmbientColor()
{
	glm::vec4 ambientColor = glm::vec4(
		material.GetAmbientColor().x * intensity,
		material.GetAmbientColor().y * intensity,
		material.GetAmbientColor().z * intensity,
		material.GetAmbientColor().w);

	return ambientColor;
}

//glm::vec3 Light::GetSpecularColor()
//{
//	return glm::vec3(material.specularValue);
//}

float Light::GetAmbientValue()
{
	return material.ambientValue;
}

//float Light::GetSpecularValue()
//{
//	return material.specularValue;
//}

void Light::SetLightColor(glm::vec4 color)
{
	material.SetBaseColor(color);
}

void Light::SetAmbientColor(glm::vec4 color)
{
	material.SetAmbientColor(color);
}

void Light::SetAmbientValue(float ambientValue)
{
	material.ambientValue = ambientValue;
}

//void Light::SetSpecualrValue(float specularValue)
//{
//	material.specularValue = specularValue;
//}

