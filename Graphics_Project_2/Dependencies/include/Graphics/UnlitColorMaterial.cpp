#include "UnlitColorMaterial.h"

glm::vec4 UnlitColorMaterial::GetBaseColor()
{
	return baseColor;
}

void UnlitColorMaterial::SetBaseColor(glm::vec4 color)
{
	this->baseColor = color;
}

void UnlitColorMaterial::UpdateMaterial(Shader* shader)
{
	shader->Bind();
	shader->SetUniform3f("solidColor", this->GetBaseColor().x, this->GetBaseColor().y, this->GetBaseColor().z);
}

void UnlitColorMaterial::ResetMaterial(Shader* shader)
{
}

BaseMaterial* UnlitColorMaterial::CloneMaterial()
{
	UnlitColorMaterial* material = new UnlitColorMaterial();

	material->SetBaseColor(this->GetBaseColor());

	return material;
}
