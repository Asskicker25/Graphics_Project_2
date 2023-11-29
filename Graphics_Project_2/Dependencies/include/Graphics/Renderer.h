#pragma once

#include "Debugger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "DebugModels.h"

enum RenderMode
{
	SHADED = 0,
	WIREFRAME = 1,
	SHADED_WIREFRAME = 2, 
};

struct ModelAndShader
{
public:
	Model* model;
	Shader* shader;
};

struct CompareDistances 
{
	const glm::vec3& cameraPos;

	explicit CompareDistances(const glm::vec3& cameraPosition) : cameraPos(cameraPosition) {}

	bool operator()(const ModelAndShader* lhs, const ModelAndShader* rhs) const 
	{
		glm::vec3 diff1 = cameraPos - lhs->model->transform.position;
		glm::vec3 diff2 = cameraPos - rhs->model->transform.position;

		return glm::dot(diff2, diff2) < glm::dot(diff1, diff1);
	}
};

class Renderer
{
private :
	
	std::vector<ModelAndShader*> nonBlendModelAndShaders;
	std::vector<ModelAndShader*> blendModelAndShaders;


	glm::vec3 backGroundColor = glm::vec3(0.1f, 0.3f, 0.4f);

	float outlineScaleValue = 0.05f;

public:

	Model* selectedModel;
	Shader* solidColorShader;
	Camera* camera = nullptr;

	Material* outlineMaterial;
	Material* wireframeMaterial;
	Material* normalsMaterial;

	DebugModels* debugCubes;

	RenderMode renderMode = SHADED;

	bool showNormals = false;

	void Initialize();

	void Clear();
	void AddModel(Model* model, Shader* shader);
	void AddModel(Model& model, Shader& shader);
	void RemoveModel(Model* model);
	void RemoveModel(Model& model);
	void SetBackgroundColor(const glm::vec3& backGroundColor);
	void Draw();
	void DrawOutline();
	void SortBlendModels();
};

