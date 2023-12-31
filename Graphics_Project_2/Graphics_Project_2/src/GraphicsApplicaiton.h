#pragma once

#include <Graphics/ApplicationWindow.h>

struct MaterialData
{
	int index;
	glm::vec4 color;
	glm::vec2 tiling;

	std::string diffusePath;
	std::string maskPath;

	MaterialData(int index)
	{
		this->index = index;
		color = glm::vec4(1.0);
		tiling = glm::vec2(1.0);
		diffusePath = "";
		maskPath = "";
	}
};

struct ModelData
{
	Model* model;
	std::string path;
	std::string shader;
	std::vector<MaterialData* > materialData;

	ModelData()
	{
		model = new Model();
		shader = "Default";
	}

	MaterialData* GetMaterialData(int index)
	{
		for (MaterialData* mat : materialData)
		{
			if (mat->index == index)
				return mat;
		}

		return nullptr;
	}
};



class GraphicsApplicaiton : public ApplicationWindow
{

private:
	const std::string& lightModelPath = "Assets/Models/DefaultSphere.fbx";

	float timeElapsed = 0;

	float cameraLerpStep = 0;
	float cameraLerpSpeed = 0.5;

	bool cameraLerp = false;

	Transform cameraStartPos;
	Transform cameraTargetPos;

	void SetSelectedModelIndex(int index);
	void SetCamera(int index);
	void HandleCameraLerp(float deltaTime);

public:

	Shader* colorMaskShader;
	Shader* uvAnimationShader;

	std::vector<ModelData*> listOfModels;
	std::vector<Model*> listOflightModels;
	std::vector<Light*> listOfLights;
	std::vector<Transform*> listOfCameraTransforms;

	int selectedIndex = 0;

	int cameraPresetIndex = 0;
	

	// Inherited via ApplicationWindow
	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;
};

