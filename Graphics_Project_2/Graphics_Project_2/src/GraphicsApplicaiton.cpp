#include "GraphicsApplicaiton.h"

void GraphicsApplicaiton::SetUp()
{

	camera->InitializeCamera(PERSPECTIVE,windowWidth,windowHeight,0.1f,300.0f,65.0f);
	camera->cameraPos = cameraPos;
	camera->cameraPitch = cameraPitch;
	camera->cameraYaw = cameraYaw;

	skyBox->meshes[0]->material = new SkyBoxMaterial();
	SkyBoxMaterial* skyboxMat = skyBox->meshes[0]->material->AsSkyBoxMaterial();

	skyboxMat->skyBoxTexture->LoadTexture({
		"Assets/Textures/Skybox/Right.jpg",
		"Assets/Textures/Skybox/Left.jpg",
		"Assets/Textures/Skybox/Up.jpg",
		"Assets/Textures/Skybox/Down.jpg",
		"Assets/Textures/Skybox/Front.jpg",
		"Assets/Textures/Skybox/Back.jpg",
		});

#pragma region Lights

	for (int i = 0; i < listOfLights.size(); i++)
	{
		listOflightModels[i]->LoadModel(lightModelPath, false);
		listOfLights[i]->InitializeLight(*listOflightModels[i], listOfLights[i]->lightType);

		//listOfLights[i]->attenuation = glm::vec4(1.0f, 0.02f, 0.001f,50.0f);

		renderer.AddModel(listOflightModels[i], &solidColorShader);
		lightManager.AddLight(*listOfLights[i]);
	}


	lightManager.AddShader(defShader);
#pragma endregion


#pragma region Models

	for (int i = 0; i < listOfModels.size(); i++)
	{
		listOfModels[i]->LoadModel(modelPaths[i]);
		renderer.AddModel(*listOfModels[i], defShader);
	}

#pragma endregion

}

void GraphicsApplicaiton::PreRender()
{
}

void GraphicsApplicaiton::PostRender()
{
}

void GraphicsApplicaiton::ProcessInput(GLFWwindow* window)
{
	std::stringstream ssTitle;
	ssTitle << "Camera Pos : "
		<< camera->cameraPos.x << " , "
		<< camera->cameraPos.y << " , "
		<< camera->cameraPos.z
		<< "  Camera Pitch : "
		<< camera->cameraPitch
		<< "  Camera Yaw : "
		<< camera->cameraYaw;

	std::string theTitle = ssTitle.str();

	glfwSetWindowTitle(window, theTitle.c_str());
}

void GraphicsApplicaiton::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
}

void GraphicsApplicaiton::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}
