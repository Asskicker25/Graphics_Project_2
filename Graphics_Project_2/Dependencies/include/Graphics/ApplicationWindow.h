#pragma once

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "LightManager.h"

#include "Material.h"
#include "UnlitColorMaterial.h"
#include "CubeMapTexture.h"
#include "SkyBoxMaterial.h"
#include "DebugCubeData.h"
#include "Timer.h"

class ApplicationWindow
{
private:
	GLFWwindow* window;

	void MoveMouse();
	void MoveCameraKeyBoard(GLFWwindow* window);
	void SetViewportSize(GLFWwindow* window, int width, int height);
	void GetCursorCallback(GLFWwindow* window, double xpos, double ypos);

public:
	float moveSpeed{ 10.0f };
	float mouseSens{ 0.1f };

	/*float cameraPitch{ 0 };
	float cameraYaw{ -90.0f };*/

	float windowWidth{ 800.0f };
	float windowHeight{ 600.0f };

	bool mouseHeld{ false };
	bool mouseCameraMove{ false };
	bool stopKeyCallback{ false };
	bool stopMouseCallback{ false };

	glm::vec2 currentMousePos{ 0 };
	glm::vec2 lastMousePos{ 0 };
	glm::vec2 mouseDeltaPos{ 0 };

	glm::mat4 view = glm::mat4(1.0f);

	/*glm::vec3 cameraPos{ 0.0f,0.0f,3.0f };
	glm::vec3 cameraFront{ 0.0f,0.0f,-1.0f };
	glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };*/

	Renderer renderer;
	LightManager lightManager;

	DebugModels* debugCubesModel;
	DebugModels* debugCubesData;
	DebugModels* debugSpheres;

	DebugCubeData cubeData;

	Shader solidColorShader;
	Shader defShader;
	Shader alphaBlendShader;
	Shader alphaCutOutShader;
	Shader skyboxShader;

	Model* skyBox;

	Camera* camera;

	ApplicationWindow();
	virtual ~ApplicationWindow() = 0;

	void InitializeWindow(int windowWidth, int windowHeight);
	void SetWindowIcon(const std::string& path);
	void SetBackgroundColor(const glm::vec3& color);

	void Render();
	virtual void SetUp() = 0;
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;
	virtual void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) = 0;
	//virtual void SetWindowIcon(GLFWwindow* window, const std::string& path) = 0;
	virtual void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) = 0;

	void MouseHeldCallback(GLFWwindow* window, int& button, int& action, int& mods);
};


