#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow()
{
	camera = new Camera();

}

ApplicationWindow::~ApplicationWindow()
{

}



void ApplicationWindow::InitializeWindow(int windowWidth, int windowHeight)
{
	if (!glfwInit())
	{
		Debugger::Print("GlfW Init failed");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	window = glfwCreateWindow(this->windowWidth, this->windowHeight, "Model Viewer", NULL, NULL);

	//User Pointer
	glfwSetWindowUserPointer(window, this);

	//SetWindowIcon(window, "Assets/Textures/Icon/Icon.png");
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->SetViewportSize(window, width, height);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->GetCursorCallback(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->MouseHeldCallback(window, button, action, mods);
			app->MouseButtonCallback(window, button, action, mods);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));

			app->KeyCallBack(window, key, scancode, action, mods);
		});


	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debugger::Print("Failed to initialize GLAD");
	}
	else
	{
		Debugger::Print("GLAD Initialized Successfully");
	}

	skyBox = new Model("res/Models/DefaultCube.fbx", false);
	skyBox->meshes[0]->material = new SkyBoxMaterial();

	SkyBoxMaterial* skyBoxMaterial = skyBox->meshes[0]->material->AsSkyBoxMaterial();

	skyBoxMaterial->skyBoxTexture->LoadTexture({
		"res/Textures/SkyBox/Right.jpg",
		"res/Textures/SkyBox/Left.jpg",
		"res/Textures/SkyBox/Up.jpg",
		"res/Textures/SkyBox/Down.jpg",
		"res/Textures/SkyBox/Front.jpg",
		"res/Textures/SkyBox/Back.jpg",
		});

	skyboxShader.LoadShader("res/Shader/SkyBox.shader");
	skyboxShader.applyModel = false;
	Debugger::Print("SkyboxShader  Id : ", skyboxShader.GetShaderId());

	solidColorShader.LoadShader("res/Shader/SolidColorShader.shader");
	Debugger::Print("SolidColorShader  Id : ", solidColorShader.GetShaderId());

	defShader.LoadShader("res/Shader/Shader.shader");
	Debugger::Print("DefShader Shader Id : ", defShader.GetShaderId());
	defShader.applyInverseModel = true;

	alphaBlendShader.LoadShader("res/Shader/Shader.shader");
	Debugger::Print("TranparentShader Shader Id : ", alphaBlendShader.GetShaderId());
	alphaBlendShader.blendMode = ALPHA_BLEND;
	alphaBlendShader.applyInverseModel = true;

	alphaCutOutShader.LoadShader("res/Shader/Shader.shader");
	Debugger::Print("AlphaCutOutShader Shader Id : ", alphaCutOutShader.GetShaderId());
	alphaCutOutShader.blendMode = ALPHA_CUTOUT;
	alphaCutOutShader.applyInverseModel = true;

	renderer.solidColorShader = &solidColorShader;
	renderer.camera = camera;


	debugCubesModel = new DebugModels("res/Models/DefaultCube.fbx");
	debugCubesData = new DebugModels(cubeData);
	debugSpheres = new DebugModels("res/Models/DefaultSphere.fbx");
	renderer.debugCubesModel = debugCubesModel;
	renderer.debugCubesData = debugCubesData;
	renderer.debugSpheres = debugSpheres;

	renderer.skyBox = new ModelAndShader({ skyBox,&skyboxShader });

	renderer.Initialize();

	lightManager.AddShader(defShader);
	lightManager.AddShader(alphaBlendShader);
	lightManager.AddShader(alphaCutOutShader);

	camera->SetCameraHeight(windowHeight);
	camera->SetCameraWidth(windowWidth);

	camera->InitializeCamera();
}

void ApplicationWindow::Render()
{
	SetUp();

	Timer::GetInstance().lastFrameTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		Timer::GetInstance().SetCurrentTime(glfwGetTime());

		//ProcessInput(window);
		MoveCameraKeyBoard(window);
		ProcessInput(window);

		renderer.Clear();
		//debugRenderer.Clear();

		view = camera->GetViewMatrix();
		//view = glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp);

		PreRender();

		lightManager.RenderLight();


		defShader.Bind();

		//Model values
		defShader.SetUniformMat("projection", camera->GetMatrix());
		defShader.SetUniformMat("view", view);
		defShader.SetUniform3f("viewPos", 
			camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

		solidColorShader.Bind();

		solidColorShader.SetUniformMat("projection", camera->GetMatrix());
		solidColorShader.SetUniformMat("view", view);

		alphaBlendShader.Bind();
		alphaBlendShader.SetUniformMat("projection", camera->GetMatrix());
		alphaBlendShader.SetUniformMat("view", view);
		alphaBlendShader.SetUniform3f("viewPos", 
			camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

		alphaCutOutShader.Bind();
		alphaCutOutShader.SetUniformMat("projection", camera->GetMatrix());
		alphaCutOutShader.SetUniformMat("view", view);
		alphaCutOutShader.SetUniform3f("viewPos", 
			camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

		view = glm::mat4(glm::mat3(view));
		skyboxShader.Bind();
		skyboxShader.SetUniformMat("projection", camera->GetMatrix());
		skyboxShader.SetUniformMat("view", view);

		renderer.Draw();
		//debugRenderer.Draw();

		debugCubesModel->Clear();
		debugCubesData->Clear();

		PostRender();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}

void ApplicationWindow::SetWindowIcon(const std::string& path)
{
	GLFWimage images[1];

	Texture::LoadImage(path.c_str(), images[0]);

	glfwSetWindowIcon(window, 1, images);
}

void ApplicationWindow::SetBackgroundColor(const glm::vec3& color)
{
	renderer.SetBackgroundColor(color);
}


void ApplicationWindow::GetCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (stopMouseCallback) return;
	if (!mouseHeld) return;


	currentMousePos.x = xpos;
	currentMousePos.y = ypos;

	mouseDeltaPos = currentMousePos - lastMousePos;
	mouseDeltaPos.y = -mouseDeltaPos.y;

	MoveMouse();

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;

	mouseCameraMove = true;
	if (!mouseCameraMove)
		std::cout << "Mouse" << mouseCameraMove << std::endl;
}

void ApplicationWindow::MoveMouse()
{
	if (stopMouseCallback) return;
	if (!mouseCameraMove) return;
	//if (lastMousePos.x == 0 && lastMousePos.y == 0) return;

	if (camera->transform.rotation.z == 180 || camera->transform.rotation.z == -180)
	{
		if (camera->transform.rotation.x > (89.0f + 180))	camera->transform.rotation.x = 89.0f + 180;
		if (camera->transform.rotation.x < -89.0f - 180)	camera->transform.rotation.x = -89.0f - 180;
	}
	else
	{
		if (camera->transform.rotation.x > 89.0f)	camera->transform.rotation.x = 89.0f;
		if (camera->transform.rotation.x < -89.0f)	camera->transform.rotation.x = -89.0f;
	}


	//std::cout << cameraYaw << std::endl;

	//std::cout << "Camera Yaw " << cameraYaw << std::endl;

	if (camera->transform.rotation.z == 180 || camera->transform.rotation.z == -180)
	{
		camera->transform.rotation.y += mouseDeltaPos.x * mouseSens;
		camera->transform.rotation.x += mouseDeltaPos.y * mouseSens;
	}
	else
	{
		camera->transform.rotation.y -= mouseDeltaPos.x * mouseSens;
		camera->transform.rotation.x += mouseDeltaPos.y * mouseSens;
	}

	camera->transform.SetRotation(camera->transform.rotation);

	//camera->SetCameraForward();
}

void ApplicationWindow::MoveCameraKeyBoard(GLFWwindow* window)
{
	if (stopKeyCallback) return;

	float speed = moveSpeed * Timer::GetInstance().deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->transform.position += camera->transform.GetForward() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->transform.position -= camera->transform.GetForward() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->transform.position -= glm::cross(camera->transform.GetForward(), camera->transform.GetUp()) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->transform.position += glm::cross(camera->transform.GetForward(), camera->transform.GetUp()) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera->transform.position -= camera->transform.GetUp() * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->transform.position += camera->transform.GetUp() * speed;
	}
}

void ApplicationWindow::SetViewportSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ApplicationWindow::MouseHeldCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
	if (stopMouseCallback) return;

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		mouseHeld = true;
	}
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
	{
		mouseHeld = false;
		mouseCameraMove = false;
	}
}