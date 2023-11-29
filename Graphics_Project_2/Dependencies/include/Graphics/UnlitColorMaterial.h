#pragma once

#include "BaseMaterial.h"

class UnlitColorMaterial : public BaseMaterial
{
private:
	glm::vec4 baseColor;

public:
	glm::vec4 GetBaseColor();
	void SetBaseColor(glm::vec4 color);

	// Inherited via BaseMaterial
	void UpdateMaterial(Shader* shader) override;
	void ResetMaterial(Shader* shader) override;

	// Inherited via BaseMaterial
	BaseMaterial* CloneMaterial() override;
};

