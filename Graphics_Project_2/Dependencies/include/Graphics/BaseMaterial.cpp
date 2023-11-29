#include "BaseMaterial.h"
#include "Material.h"
#include "UnlitColorMaterial.h"

Material* BaseMaterial::AsMaterial()
{
	return (Material*)this;
}

UnlitColorMaterial* BaseMaterial::AsUnlitMaterial()
{
	return (UnlitColorMaterial*)this;
}
