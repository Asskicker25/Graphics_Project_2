#include "Debugger.h"
#include "Vertex.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "TextureData.h"
#include "Material.h"
#include "Triangle.h"

class Renderer;

#pragma once
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector< unsigned int> indices;
	std::vector< Triangles > triangles;
	std::vector< Texture*> textures;

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector< unsigned int>& indices);

	void SetRenderer(Renderer* renderer);

	void DrawShadedMesh(Shader* shader, BaseMaterial* material,  bool isWireFrame = false);
	void DrawSolidColorMesh(Shader* shader, glm::vec3 color, bool isWireFrame = false);
	void DrawNormals(Shader* shader, glm::vec3 color, glm::mat4 transformMatrix);

	void UpdateVertices();
	VertexArray VAO;

private:
	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexLayout layout;

	Renderer* renderer;

	//unsigned int VAO, VBO, EBO;

	void SetupMesh();
	void CalculateTriangles();
};

