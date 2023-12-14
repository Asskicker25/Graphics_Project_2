#include "Renderer.h"
#include "UnlitColorMaterial.h"


void Renderer::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	outlineMaterial = new Material();
	wireframeMaterial = new Material();
	normalsMaterial = new Material();

	outlineMaterial->SetBaseColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	wireframeMaterial->SetBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	normalsMaterial->SetBaseColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

}

void Renderer::Clear()
{
	GLCALL(glClearColor(backGroundColor.x, backGroundColor.y, backGroundColor.z, 1.0f));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(0x00);
}

void Renderer::AddModel(Model* model, Shader* shader)
{
	if (shader->blendMode == OPAQUE || shader->blendMode == ALPHA_CUTOUT)
	{
		nonBlendModelAndShaders.push_back(new  ModelAndShader{ model,shader });
	}
	else if (shader->blendMode == ALPHA_BLEND)
	{
		blendModelAndShaders.push_back(new  ModelAndShader{ model,shader });
	}

	model->SetRenderer(this);
}

void Renderer::AddModel(Model& model, Shader& shader)
{
	if (shader.blendMode == OPAQUE || shader.blendMode == ALPHA_CUTOUT)
	{
		nonBlendModelAndShaders.push_back(new  ModelAndShader{ &model,&shader });
	}
	else if (shader.blendMode == ALPHA_BLEND)
	{
		blendModelAndShaders.push_back(new  ModelAndShader{ &model, &shader });
	}

	model.SetRenderer(this);

}

void Renderer::RemoveModel(Model* model)
{

	for (ModelAndShader* modelShader : nonBlendModelAndShaders)
	{
		if (modelShader->model == model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), modelShader), nonBlendModelAndShaders.end());
			return;
		}
	}

	for (ModelAndShader* modelShader : blendModelAndShaders)
	{
		if (modelShader->model == model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), modelShader), nonBlendModelAndShaders.end());
			return;
		}
	}
}

void Renderer::RemoveModel(Model& model)
{

	for (ModelAndShader* modelShader : nonBlendModelAndShaders)
	{
		if (modelShader->model == &model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), modelShader), nonBlendModelAndShaders.end());
			return;
		}
	}

	for (ModelAndShader* modelShader : blendModelAndShaders)
	{
		if (modelShader->model == &model)
		{
			nonBlendModelAndShaders.erase(std::remove(nonBlendModelAndShaders.begin(),
				nonBlendModelAndShaders.end(), modelShader), nonBlendModelAndShaders.end());
			return;
		}
	}
}

void Renderer::SetBackgroundColor(const glm::vec3& backGroundColor)
{
	this->backGroundColor = backGroundColor;
}

void Renderer::Draw()
{
	glDepthFunc(GL_LEQUAL);
	skyBox->model->DrawShaded(skyBox->shader);
	glDepthFunc(GL_LESS);

	ModelAndShader* tempSelectedModel = nullptr;

	for (unsigned int i = 0; i < nonBlendModelAndShaders.size(); i++)
	{
		if (selectedModel != nullptr && nonBlendModelAndShaders[i]->model == selectedModel)
		{
			tempSelectedModel = nonBlendModelAndShaders[i];
			continue;
		}

		nonBlendModelAndShaders[i]->model->Draw(nonBlendModelAndShaders[i]->shader);
	}

	SortBlendModels();

	for (ModelAndShader* model : blendModelAndShaders)
	{
		if (selectedModel != nullptr && model->model == selectedModel)
		{
			tempSelectedModel = model;
			continue;
		}
		model->model->Draw(model->shader);
	}


	if (tempSelectedModel != nullptr)

	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		tempSelectedModel->model->Draw(tempSelectedModel->shader);

		if (showNormals)
		{
			tempSelectedModel->model->DrawNormals();
		}

		DrawOutline();
	}


}

void Renderer::DrawOutline()
{

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	bool loadTextures = selectedModel->loadTextures;

	glm::vec3 currentScale = selectedModel->transform.scale;

	selectedModel->transform.scale.x += selectedModel->transform.scale.x * outlineScaleValue;
	selectedModel->transform.scale.y += selectedModel->transform.scale.y * outlineScaleValue;
	selectedModel->transform.scale.z += selectedModel->transform.scale.z * outlineScaleValue;

	selectedModel->loadTextures = false;

	selectedModel->DrawSolidColor(solidColorShader, outlineMaterial->GetBaseColor());

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);

	selectedModel->transform.SetScale(currentScale);
	selectedModel->loadTextures = loadTextures;
}

void Renderer::SortBlendModels()
{
	CompareDistances compareDistance(camera->transform.position);
	std::sort(blendModelAndShaders.begin(), blendModelAndShaders.end(), compareDistance);
}

const glm::vec3& Renderer::GetNormalsScale()
{
	return this->normalsScale;
}

void Renderer::SetNormalsLineScale(const glm::vec3& scale)
{
	this->normalsScale = scale;
}

void Renderer::DrawAABB(const modelAABB& aabb, glm::vec4 color, bool drawFromModel)
{
	glm::vec3 targetExtents = 0.5f * (aabb.max - aabb.min);
	glm::vec3 center = 0.5f * (aabb.max + aabb.min);

	Model* cube;

	if(drawFromModel)
	{
		cube = debugCubesModel->DrawDebugModel();
	}
	else
	{
		cube = debugCubesData->DrawDebugModel();
	}
	
	cube->SetRenderer(this);
	cube->transform.SetPosition(center);
	cube->transform.SetRotation(glm::vec3(0));
	cube->transform.SetScale(targetExtents);
	cube->DrawWireframe(color);
}

void Renderer::DrawCube(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale, const glm::vec4 color, bool drawFromModel)
{
	Model* cube;

	if (drawFromModel)
	{
		cube = debugCubesModel->DrawDebugModel();
	}
	else
	{
		cube = debugCubesData->DrawDebugModel();
	}
	cube->SetRenderer(this);
	cube->transform.SetPosition(pos);
	cube->transform.SetRotation(rot);
	cube->transform.SetScale(scale);
	cube->DrawWireframe(color);
}

void Renderer::DrawSphere(const glm::vec3 center, float radius, glm::vec4 color)
{
	Model* sphere = debugSpheres->DrawDebugModel();
	sphere->SetRenderer(this);
	sphere->transform.SetPosition(center);
	sphere->transform.SetScale(glm::vec3(radius));
	sphere->DrawWireframe(color);
}

