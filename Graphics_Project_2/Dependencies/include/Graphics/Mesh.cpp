#include "Mesh.h"
#include "Renderer.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	VAO.Bind();

	this->vertices = vertices;
	this->indices = indices;

	/*if (textures.size() != 0)
	{
		material.diffuseTexture = textures[0];
		material.specularTexture = textures[1];
	}*/

	//this->textures = textures;

	SetupMesh();
}

void Mesh::SetRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

void Mesh::DrawShadedMesh(Shader* shader, BaseMaterial* material, bool isWireframe)
{
	material->UpdateMaterial(shader);

	VAO.Bind();

	if (isWireframe)
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
	else
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

	/*material->diffuseTexture->Unbind();
	material->specularTexture->Unbind();*/
	VAO.UnBind();

}

void Mesh::DrawSolidColorMesh(Shader* shader, glm::vec3 color, bool isWireFrame)
{
	shader->Bind();
	shader->SetUniform3f("solidColor", color.x, color.y, color.z);

	VAO.Bind();

	if (isWireFrame)
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
	else
	{
		GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

	VAO.UnBind();
}

void Mesh::DrawNormals(Shader* shader, glm::vec3 color, glm::mat4 transformMatrix)
{
	shader->Bind();
	shader->SetUniform3f("solidColor", color.x, color.y, color.z);


	for (int i = 0; i < triangles.size(); i++)
	{
		Model* model = renderer->debugCubesModel->DrawDebugModel();

		model->transform.SetScale(renderer->GetNormalsScale());

		glm::vec3 transformedCenter = transformMatrix * glm::vec4(triangles[i].center, 1.0f);
		glm::vec3 transformedNormal =glm::normalize( transformMatrix * glm::vec4(triangles[i].normal,0));
		glm::vec3 pos = transformedCenter + (transformedNormal * model->transform.scale.y * 0.5f);
		//glm::vec3 pos = transformedCenter ;

		model->transform.SetPosition(pos); 

		model->transform.SetOrientationFromDirections(transformedNormal,
			transformMatrix * glm::vec4(triangles[i].tangent,0.0f));

		
		model->DrawSolidColor(shader, color);
		//model->Draw(shader);
	}
}


void Mesh::UpdateVertices()
{
	VAO.Bind();
	VBO.UpdateVertexData(vertices.size() * sizeof(Vertex), &vertices[0]);
	VAO.AddBuffer(VBO, layout);
	VAO.UnBind();
}

void Mesh::SetupMesh()
{
	CalculateTriangles();

	VAO.Bind();
	VBO.Setup(vertices.size() * sizeof(Vertex), &vertices[0]);
	IBO.Setup(indices.size(), &indices[0]);

	//Position
	layout.AddLayout<float>(3);

	//Normals
	layout.AddLayout<float>(3);

	//Texture
	layout.AddLayout<float>(2);

	layout.AddLayout<float>(4);

	VAO.AddBuffer(VBO, layout);

	VAO.UnBind();
}

void Mesh::CalculateTriangles()
{
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		Triangles triangle;
		triangle.v1 = vertices[indices[i]].positions;
		triangle.v2 = vertices[indices[i + 1]].positions;
		triangle.v3 = vertices[indices[i + 2]].positions;

		triangle.center = (triangle.v1 + triangle.v2 + triangle.v3) / 3.0f;

		triangle.normal = (vertices[indices[i]].normals +
			vertices[indices[i + 1]].normals +
			vertices[indices[i + 2]].normals) / 3.0f;

		glm::vec3 edge = triangle.v2 - triangle.v1;

		triangle.tangent = glm::normalize(glm::cross(triangle.normal, edge));

		triangles.push_back(triangle);
	}

}



