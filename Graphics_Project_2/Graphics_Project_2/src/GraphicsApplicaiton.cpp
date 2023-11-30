#include "GraphicsApplicaiton.h"



void GraphicsApplicaiton::SetUp()
{

	renderer.SetNormalsLineScale(glm::vec3(0.01, 3.0, 0.01));

#pragma region Shader

	colorMaskShader = new Shader("res/Shader/ColorMaskShader.shader");
	colorMaskShader->blendMode = OPAQUE;
	colorMaskShader->applyInverseModel = true;
	Debugger::Print("ColorMaskShaderID : ", colorMaskShader->GetShaderId());


	uvAnimationShader = new Shader("res/Shader/UVAnimationShader.shader");
	uvAnimationShader->blendMode = OPAQUE;
	uvAnimationShader->applyInverseModel = true;
	Debugger::Print("UvAnimationShader Shader Id : ", uvAnimationShader->GetShaderId());

#pragma endregion


	camera->InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 300.0f, 65.0f);
	camera->cameraPos = cameraPos;
	camera->cameraPitch = cameraPitch;
	camera->cameraYaw = cameraYaw;
	moveSpeed = 50;

	renderer.renderMode = SHADED;
	renderer.showNormals = false;

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

	lightManager.AddShader(colorMaskShader);
	lightManager.AddShader(uvAnimationShader);

#pragma endregion


#pragma region Models

	int index = 0;

	for (ModelData* modelData : listOfModels)
	{
		modelData->model->LoadModel(modelData->path);

		for (MaterialData* matData : modelData->materialData)
		{
			BaseMaterial* material = modelData->model->meshes[matData->index]->material;

			if (modelData->shader == "Default" || modelData->shader == "AlphaBlend" || modelData->shader == "ColorMask" ||
				modelData->shader == "UVAnim" || modelData->shader == "AlphaCutOut")
			{
				material->AsMaterial()->SetBaseColor(matData->color);
				material->AsMaterial()->textureTiling = matData->tiling;
				if (matData->diffusePath != "")
				{
					material->AsMaterial()->diffuseTexture->LoadTexture(matData->diffusePath);
				}
				if (matData->maskPath != "")
				{
					material->AsMaterial()->alphaMask->LoadTexture(matData->maskPath);
					material->AsMaterial()->useMaskTexture = true;
				}
			}
		}

		if (modelData->shader == "Default")
		{
			renderer.AddModel(modelData->model, &defShader);
		}
		else if (modelData->shader == "AlphaBlend")
		{
			renderer.AddModel(modelData->model, &alphaBlendShader);
		}
		else if (modelData->shader == "ColorMask")
		{
			renderer.AddModel(modelData->model, colorMaskShader);
		}
		else if (modelData->shader == "UVAnim")
		{
			renderer.AddModel(modelData->model, uvAnimationShader);
		}
		else if (modelData->shader == "AlphaCutOut")
		{
			renderer.AddModel(modelData->model, &alphaCutOutShader);
		}

		SetSelectedModelIndex(index);

		index++;
	}

#pragma endregion

}

void GraphicsApplicaiton::PreRender()
{

#pragma region Shader

	colorMaskShader->Bind();
	colorMaskShader->SetUniformMat("projection", camera->GetMatrix());
	colorMaskShader->SetUniformMat("view", view);
	colorMaskShader->SetUniform3f("viewPos", camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

	uvAnimationShader->Bind();
	uvAnimationShader->SetUniformMat("projection", camera->GetMatrix());
	uvAnimationShader->SetUniformMat("view", view);
	uvAnimationShader->SetUniform3f("viewPos", camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

	uvAnimationShader->SetUniform1f("time", timeElapsed);

#pragma endregion



}

void GraphicsApplicaiton::PostRender()
{
	timeElapsed += deltaTime;
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
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_RIGHT)
		{
			SetSelectedModelIndex(++selectedIndex);
		}
		else if (key == GLFW_KEY_LEFT)
		{
			SetSelectedModelIndex(--selectedIndex);
		}
		else if (key == GLFW_KEY_N)
		{
			renderer.showNormals = !renderer.showNormals;
		}
		else if (key == GLFW_KEY_1)
		{
			renderer.renderMode = SHADED;
		}
		else if (key == GLFW_KEY_2)
		{
			renderer.renderMode = WIREFRAME;
		}
		else if (key == GLFW_KEY_3)
		{
			renderer.renderMode = SHADED_WIREFRAME;
		}
		else if (key == GLFW_KEY_UP)
		{
			renderer.selectedModel = nullptr;
		}
	}
}

void GraphicsApplicaiton::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}

void GraphicsApplicaiton::SetSelectedModelIndex(int index)
{
	if (index >= listOfModels.size())
	{
		index = 0;
	}
	else if (index <= 0)
	{
		index = listOfModels.size() - 1;
	}

	selectedIndex = index;
	renderer.selectedModel = listOfModels[selectedIndex]->model;
}
