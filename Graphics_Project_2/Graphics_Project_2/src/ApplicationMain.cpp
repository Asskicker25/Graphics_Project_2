#include "GraphicsApplicaiton.h"

void StartApplication(const std::string& filePath);
void ReadFile(const std::string& filePath);

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
			else if (model != nullptr)
			{
				application.listOfModels.push_back(model);
			}
			model = new Model();
			light = new Light();

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
			else if (model != nullptr)
			{
				application.listOfModels.push_back(model);
			}

			light = nullptr;
			model = new Model();
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
			line.erase(0, line.find("{") + 1);
			line.erase(line.find("}"));

			std::string result;
			for (char c : line) {
				if (c != ' ') {
					result += c;
				}
			}

			if (section == "Model")
			{
				application.modelPaths.push_back(result);
			}
			continue;

		}
#pragma endregion

#pragma region ModelId
		if (line.find("Model Id") != std::string::npos)
		{
			line.erase(0, line.find("{") + 1);
			line.erase(line.find("}"));

			model->modelId = line;
			continue;

		}
#pragma endregion

#pragma region Wireframe
		if (line.find("Wireframe") != std::string::npos)
		{
			line.erase(0, line.find("{") + 1);
			line.erase(line.find("}"));

			model->modelId = line;

			std::string result;
			for (char c : line) {
				if (c != ' ') {
					result += c;
				}
			}

			if (result == "true" || "True" || "TRUE")
			{
				model->isWireframe = true;
			}
			else
			{
				model->isWireframe = false;
			}
			continue;

		}
#pragma endregion

#pragma region LightType

		if (line.find("Type") != std::string::npos)
		{
			line.erase(0, line.find("{") + 1);
			line.erase(line.find("}"));

			if (line == " Directional ")
			{
				light->lightType = LightType::Directional;
			}
			else if (line == " Spot ")
			{
				light->lightType = LightType::Spot;
			}
			else if (line == " Point ")
			{
				light->lightType = LightType::Point;
			}
			continue;

		}

#pragma endregion

#pragma region LightRadius

		if (line.find("Radius") != std::string::npos)
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

			glm::vec4 atten = glm::vec4(0.0f);

			atten.x = 1.0f;
			atten.z = 0.02;
			atten.y = -atten.z * values[0];

			light->attenuation = atten;

			continue;

		}
#pragma endregion

#pragma region LightAngle

		if (line.find("InnerOuterAngle") != std::string::npos)
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

			glm::vec4 atten = glm::vec4(0.0f);



			light->innerAngle = values[0];
			light->outerAngle = values[1];

			continue;

		}
#pragma endregion

#pragma region Intensity

		if (line.find("Intensity") != std::string::npos)
		{
			line.erase(0, line.find("{") + 1);
			line.erase(line.find("}"));

			std::stringstream ss(line);
			double value;

			ss >> value;

			light->intensity = value;

			continue;

		}

#pragma endregion

#pragma region Color

		if (line.find("Color") != std::string::npos)
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

#pragma region Position

		if (line.find("Position") != std::string::npos)
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

			if (section == "Camera")
			{
				application.camera->cameraPos = glm::vec3(values[0], values[1], values[2]);
			}
			else
			{
				model->transform.SetPosition(glm::vec3(glm::vec3(values[0], values[1], values[2])));
			}

			continue;

		}
#pragma endregion

#pragma region Rotation
		if (line.find("Rotation") != std::string::npos)
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
			if (section == "Camera")
			{
				application.camera->cameraPitch = values[0];
				application.camera->cameraYaw = values[1];
			}
			else
			{
				model->transform.SetRotation(glm::vec3(glm::vec3(values[0], values[1], values[2])));
			}
			continue;
		}
#pragma endregion

#pragma region Scale
		if (line.find("Scale") != std::string::npos)
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
			model->transform.SetScale(glm::vec3(glm::vec3(values[0], values[1], values[2])));
			continue;
		}
#pragma endregion

	}

	if (light != nullptr)
	{
		application.listOfLights.push_back(light);
		application.listOflightModels.push_back(model);
	}
	else if (model != nullptr)
	{
		application.listOfModels.push_back(model);
	}

}

void StartApplication(const std::string& filePath)
{
	ReadFile(filePath);

	application.InitializeWindow(1366, 728);

	application.Render();
}