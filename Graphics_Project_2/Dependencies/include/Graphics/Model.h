#pragma once
#include "Debugger.h"
#include "Mesh.h"
#include "Transform.h"
#include "BaseMaterial.h"

class Renderer;

struct MeshAndMaterial
{
	std::shared_ptr<Mesh> mesh;
	BaseMaterial* material;
};


class Model
{
public: 
	Model();
	Model(const Model& model);
	~Model();
	Model(const std::string& path, bool loadTextures = true);

	void SetRenderer(Renderer* renderer);
	void Draw(Shader* shader);
	void DrawSolidColor(Shader* shader, glm::vec3 color);
	Model* CopyFromModel(const Model& model);

	std::string modelId;
	bool isActive = true;
	bool isWireframe = false;

	std::vector<MeshAndMaterial*> meshes;
	std::vector<Texture*> texturesLoaded;
	std::string directory;

	Transform transform;
	//Material material;

	void LoadModel(const std::string& path, bool loadTextures = true);
	void DrawNormals();
	void DrawShaded(Shader* shader);

	bool loadTextures;

private:

	Renderer* renderer = nullptr;

	void ProcessNode(aiNode* node, const aiScene* scene);
	MeshAndMaterial* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Texture* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Texture* LoadDefaultMaterialTextures(aiTextureType type, std::string typeName);

	void DrawShaded(MeshAndMaterial* mesh, Shader* shader);
	void DrawWireframe(MeshAndMaterial* mesh, Shader* shader);
	void DrawNormals(MeshAndMaterial* mesh, Shader* shader);
};

