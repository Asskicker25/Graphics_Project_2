#pragma once

#include "Shader.h"

class Material;
class UnlitColorMaterial;
class SkyBoxMaterial;


class BaseMaterial
{
public:
	virtual void UpdateMaterial(Shader* shader) = 0;
	virtual void ResetMaterial(Shader* shader) = 0;
	virtual BaseMaterial* CloneMaterial() = 0;

	Material* AsMaterial();
	UnlitColorMaterial* AsUnlitMaterial();
	SkyBoxMaterial* AsSkyBoxMaterial();

};

