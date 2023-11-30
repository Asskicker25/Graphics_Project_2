#pragma once

#include <Graphics/ApplicationWindow.h>

class GraphicsApplicaiton : public ApplicationWindow
{

private:
	const std::string& lightModelPath = "Assets/Models/DefaultSphere.fbx";

public:

	std::vector<Model*> listOfModels;
	std::vector<Model*> listOflightModels;
	std::vector<std::string> modelPaths;
	std::vector<Light*> listOfLights;

	glm::vec3 cameraPos;
	float cameraPitch;
	float cameraYaw;

	// Inherited via ApplicationWindow
	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;
};

