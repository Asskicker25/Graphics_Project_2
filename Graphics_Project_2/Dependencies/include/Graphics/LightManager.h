#pragma once

#include "Debugger.h"
#include "Light.h"
#include "Shader.h"

class LightManager
{
private:
	std::vector<Light*> lights;
	std::vector<Shader*> shaders;
	Shader* lightShader = nullptr;

	static const unsigned int NUMBER_OF_LIGHTS_IN_USE = 15;

public:

	LightManager();
	~LightManager();

	void SetLightShader(Shader& lightShader);
	void AddLight(Light* light);
	void AddShader(Shader* shader);
	void RemoveLight(Light* light);

	void AddLight(Light& light);
	void AddShader(Shader& shader);
	void RemoveLight(Light& light);

	void RenderLight();
};

