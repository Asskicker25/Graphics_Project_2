#include "GraphicsApplicaiton.h"

void StartApplication(const std::string& filePath);
void ReadFile(const std::string& filePath);

std::vector<float> GetValues(std::string line);
std::string GetString(std::string line, bool excludeSpace = false);
int GetMaterialIndex(std::string line);
float GetFloat(std::string line);
bool GetBool(std::string line);

GraphicsApplicaiton application;

int main(int argc, char* argv[])
{
	Debugger::print = true;

	if (argc != 2)
	{
		std::cout << "(You can also drop file onto the exe)" << std::endl;
		std::cout << std::endl;

		StartApplication("Scene.txt");

		return -1;
	}

	StartApplication(std::string(argv[1]));
	return -1;
}

void ReadFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	std::string line;
	std::string section;

	ModelData* modelData = nullptr;
	Model* model = nullptr;
	Light* light = nullptr;

	if (!file.is_open())
	{
		Debugger::Print("File doesn't exit!!!");
		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		if (line.find("//") != std::string::npos) continue;

		if (line.find("#Light") != std::string::npos)
		{
			if (light != nullptr)
			{
				model->transform.SetScale(glm::vec3(0.1f));
				application.listOfLights.push_back(light);
				application.listOflightModels.push_back(model);
			}
			else if (modelData != nullptr)
			{
				application.listOfModels.push_back(modelData);
			}

			delete model;
			delete light;
			delete modelData;

			model = new Model();
			light = new Light();
			modelData = nullptr;

			section = "Light";
			continue;
		}
		else if (line.find("#Model") != std::string::npos)
		{

			if (light != nullptr)
			{
				model->transform.SetScale(glm::vec3(0.1f));
				application.listOfLights.push_back(light);
				application.listOflightModels.push_back(model);
			}
			else if (modelData != nullptr)
			{
				application.listOfModels.push_back(modelData);
			}

			light = nullptr;
			model = nullptr;
			modelData = new ModelData();
			section = "Model";
			continue;
		}
		else if (line.find("#Camera") != std::string::npos)
		{
			section = "Camera";
			continue;
		}

#pragma region Modelpath
		if (line.find("Model path") != std::string::npos)
		{
			if (section == "Model")
			{
				modelData->path = GetString(line, true);
			}
			continue;

		}
#pragma endregion

#pragma region ModelId
		if (line.find("Model Id") != std::string::npos)
		{

			modelData->model->modelId = GetString(line);
			continue;

		}
#pragma endregion

#pragma region Shader
		if (line.find("Shader") != std::string::npos)
		{
			modelData->shader = GetString(line, true);
		}

#pragma endregion

#pragma region Wireframe
		if (line.find("Wireframe") != std::string::npos)
		{
			modelData->model->isWireframe = GetBool(line);
			continue;

		}
#pragma endregion

#pragma region LightType

		if (line.find("Type") != std::string::npos)
		{
			line = GetString(line, true);

			if (line == "Directional")
			{
				light->lightType = LightType::Directional;
			}
			else if (line == "Spot")
			{
				light->lightType = LightType::Spot;
			}
			else if (line == "Point")
			{
				light->lightType = LightType::Point;
			}
			continue;

		}

#pragma endregion

#pragma region LightRadius

		if (line.find("Radius") != std::string::npos)
		{

			glm::vec4 atten = glm::vec4(0.0f);

			atten.x = 1.0f;
			atten.z = 0.02;
			atten.y = -atten.z * GetFloat(line);

			light->attenuation = atten;

			continue;

		}
#pragma endregion

#pragma region LightAngle

		if (line.find("InnerOuterAngle") != std::string::npos)
		{
			std::vector<float> values = GetValues(line);

			glm::vec4 atten = glm::vec4(0.0f);

			light->innerAngle = values[0];
			light->outerAngle = values[1];

			continue;

		}
#pragma endregion

#pragma region Intensity

		if (line.find("Intensity") != std::string::npos)
		{
			light->intensity = GetFloat(line);

			continue;

		}

#pragma endregion

#pragma region Color

		if (line.find("Color") != std::string::npos)
		{
			std::vector<float> values = GetValues(line);

			if (section == "Light")
			{
				light->SetLightColor(glm::vec4(values[0], values[1], values[2], values[3]));
			}
			else if (section == "Model")
			{
			}
			continue;

		}
#pragma endregion

#pragma region MaterialColor

		if (line.find("MaterialCol") != std::string::npos)
		{
			int materialIndex = GetMaterialIndex(line);

			std::vector<float> values = GetValues(line);

			if (section == "Model")
			{
				MaterialData* matData = modelData->GetMaterialData(materialIndex);

				if (matData == nullptr)
				{
					matData = new MaterialData(materialIndex);
					modelData->materialData.push_back(matData);
				}

				matData->color = glm::vec4(values[0], values[1], values[2], values[3]);

			}

			continue;
		}

#pragma endregion

#pragma region MaterialTiling


		if (line.find("MaterialTiling") != std::string::npos)
		{
			int materialIndex = GetMaterialIndex(line);

			std::vector<float> values = GetValues(line);

			if (section == "Model")
			{
				MaterialData* matData = modelData->GetMaterialData(materialIndex);

				if (matData == nullptr)
				{
					matData = new MaterialData(materialIndex);
					modelData->materialData.push_back(matData);
				}

				matData->tiling = glm::vec2(values[0], values[1]);

			}

			continue;
		}

#pragma endregion

#pragma region MaterialDiffuse


		if (line.find("MaterialDiffuse") != std::string::npos)
		{
			int materialIndex = GetMaterialIndex(line);

			if (section == "Model")
			{
				MaterialData* matData = modelData->GetMaterialData(materialIndex);

				if (matData == nullptr)
				{
					matData = new MaterialData(materialIndex);
					modelData->materialData.push_back(matData);
				}

				matData->diffusePath = GetString(line, true);

			}

			continue;
		}

#pragma endregion

#pragma region MaterialMask


		if (line.find("MaterialMask") != std::string::npos)
		{
			int materialIndex = GetMaterialIndex(line);

			if (section == "Model")
			{
				MaterialData* matData = modelData->GetMaterialData(materialIndex);

				if (matData == nullptr)
				{
					matData = new MaterialData(materialIndex);
					modelData->materialData.push_back(matData);
				}

				matData->maskPath = GetString(line, true);

			}

			continue;
		}

#pragma endregion

#pragma region Position

		if (line.find("Position") != std::string::npos)
		{
			std::vector<float> values = GetValues(line);

			if (section == "Camera")
			{
				application.cameraPos = glm::vec3(values[0], values[1], values[2]);
			}
			else
			{
				if (section == "Light")
				{
					model->transform.SetPosition(glm::vec3(glm::vec3(values[0], values[1], values[2])));
				}
				else
				{
					modelData->model->transform.SetPosition(glm::vec3(glm::vec3(values[0], values[1], values[2])));
				}

			}

			continue;

		}
#pragma endregion

#pragma region Rotation
		if (line.find("Rotation") != std::string::npos)
		{
			std::vector<float> values = GetValues(line);

			if (section == "Camera")
			{
				application.cameraPitch = values[0];
				application.cameraYaw = values[1];

			}
			else
			{
				if (section == "Light")
				{
					model->transform.SetRotation(glm::vec3(glm::vec3(values[0], values[1], values[2])));
				}
				else
				{
					modelData->model->transform.SetRotation(glm::vec3(glm::vec3(values[0], values[1], values[2])));
				}
			}
			continue;
		}
#pragma endregion

#pragma region Scale
		if (line.find("Scale") != std::string::npos)
		{
			std::vector<float> values = GetValues(line);

			if (section == "Light")
			{
				model->transform.SetScale(glm::vec3(glm::vec3(values[0], values[1], values[2])));
			}
			else
			{
				modelData->model->transform.SetScale(glm::vec3(glm::vec3(values[0], values[1], values[2])));
			}

			continue;
		}
#pragma endregion

	}

	if (light != nullptr)
	{
		application.listOfLights.push_back(light);
		application.listOflightModels.push_back(model);
	}
	else if (modelData != nullptr)
	{
		application.listOfModels.push_back(modelData);
	}

}

std::vector<float> GetValues(std::string line)
{
	line.erase(0, line.find("{") + 1);
	line.erase(line.find("}"));

	std::stringstream ss(line);
	double value;
	char comma;

	std::vector<float> values;

	while (ss >> value)
	{
		ss >> comma;

		values.push_back((float)value);
	}

	return values;
}

std::string GetString(std::string line, bool excludeSpace)
{

	line.erase(0, line.find("{") + 1);
	line.erase(line.find("}"));

	if (excludeSpace)
	{
		std::string result;
		for (char c : line) {
			if (c != ' ') {
				result += c;
			}
		}
		return result;
	}
	else
	{
		return line;
	}
}

int GetMaterialIndex(std::string line)
{
	int materialIndex;

	size_t startBracket = line.find("[") + 1;
	size_t endBracket = line.find("]");
	std::string valueStr = line.substr(startBracket, endBracket - startBracket);
	std::stringstream valueStream(valueStr);
	valueStream >> materialIndex;

	return materialIndex;
}

float GetFloat(std::string line)
{
	line.erase(0, line.find("{") + 1);
	line.erase(line.find("}"));

	std::stringstream ss(line);
	double value;

	ss >> value;

	return value;
}

bool GetBool(std::string line)
{
	line = GetString(line, true);

	if (line == "true" || "True" || "TRUE")
	{
		return true;
	}

	return false;
}

void StartApplication(const std::string& filePath)
{
	ReadFile(filePath);

	application.InitializeWindow(1366, 728);

	application.Render();
}