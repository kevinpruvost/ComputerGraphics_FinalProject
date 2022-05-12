/*****************************************************************//**
 * \file   main.cpp
 * \brief  Main of the Assignment 1 of the Fundamentals
 *         of Computer Graphics course
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/

// C++ includes
#include <format>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Project includes
#include "OGL_Implementation\Window.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\Obj.hpp"
#include "OGL_Implementation\Camera.hpp"
#include "OGL_Implementation\Mesh\Mesh.hpp"
#include "OGL_Implementation\GUI.hpp"
#include "OGL_Implementation\Entity\Entity.hpp"
#include "OGL_Implementation\OpenGL_Timer.hpp"
#include "OGL_Implementation\DebugInfo\FpsCounter.hpp"
#include "OGL_Implementation\Rendering\Rendering.hpp"
#include "OGL_Implementation\Text\Text.hpp"
#include "OGL_Implementation\Light\Light.hpp"

#include "OGL_Implementation\Entity\ParticleSystem\Snow\ParticleSystem_Snow.hpp"
#include "OGL_Implementation\Image\Image2D.hpp"

#include "OGL_Implementation\DebugInfo\AxisDisplayer.hpp"

#include "Constants.hpp"

#include <omp.h>
#include <thread>

int main()
{
	omp_set_num_threads(omp_get_num_procs());

	Window * window = Window::Init(Constants::Window::windowName, Constants::Paths::windowIcon);
	if (!window)
		return EXIT_FAILURE;

	Rendering::Init();

	Texture sunTexture;
	if (!sunTexture.GenerateTexture("resources/Textures/sun.jpg"))
	{
		LOG_PRINT(stderr, "Couldn't load texture '%s'\n", "resources/Textures/sun.jpg");
		return EXIT_FAILURE;
	}

	Obj obj;
	if (!obj.TryLoad(Constants::Paths::Models::Icosahedron::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Icosahedron::objFile);
		return EXIT_FAILURE;
	}
	obj.GenerateNormals(false);
	Mesh meshObjNotSmooth = GenerateMesh(obj.verticesPos, obj.verticesNormals, obj.faces);
	obj.GenerateNormals(true);
	Mesh meshObjSmooth = GenerateMesh(obj.verticesPos, obj.verticesNormals, obj.faces);

	Entity entity(meshObjNotSmooth,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::faceShaderVertex));
	Material & entityMaterial = entity.AddMaterial();
	entity.SetShaderAttribute("isNormalFlat", 1);
	entityMaterial.diffuseColor = glm::vec3(1.0f);
	entityMaterial.specularColor = glm::vec3(0.0f);

	entity.scale = glm::vec3(1.0f);

	Mesh sphereMesh = GenerateMeshSphere();
	PointLight sun(sphereMesh);
	sun.SetTexture(sunTexture);
	sun.pos = { 0.0f, -3.0f, 0.0f };
	sun.ChangeSpecular(glm::vec3(1.0f));
	sun.ChangeDiffuse(glm::vec3(1.0f));
	sun.ChangeAmbient(glm::vec3(1.0f));

	Camera camera(window->windowWidth(), window->windowHeight(), -2.0f, 4.0f, 5.0f);
	camera.MovementSpeed *= 1.0f;
	mainCamera = &camera;

	camera.LookAt(entity.pos);

	bool cameraLock = false;
	// GUI
	GUI gui(window->window);
	// Creating Second Window
	bool enableGui = true;
	bool autoRotation = false;
	gui.AddCallback([&]() {
		const float width = 320.0f;
		const float height = 475.0f;
		ImGui::SetNextWindowSize({ width, height }, ImGuiCond_::ImGuiCond_Always);
		ImGui::SetNextWindowPos(
			{ImGui::GetIO().DisplaySize.x - 20.0f - width, 20.0f},
			ImGuiCond_::ImGuiCond_Always);
		ImGui::Begin("Settings:");

		ImGui::Checkbox("Enable/Disable GUI (Press T)", &enableGui);
		if (ImGui::TreeNodeEx("General", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text(std::format("FPS: {}", GetFpsCount(window->DeltaTimeNoMultiplier(), 0.5f)).c_str());
			ImGui::SliderInt("FPS cap", (int *)&window->fpsCap, 0, 60);
			ImGui::SliderFloat("Time Multiplier", const_cast<float *>(&window->GetTimeMultiplier()), 0.0f, 5.0f);

			int displayMode = DisplayMode - 1;
			const char * const displayModeItems[7] = { "Vertices", "Wireframes", "Vertices/Wireframes", "Faces", "Vertices/Faces", "Wireframes/Faces", "All" };
			if (ImGui::Combo("Display Mode", &displayMode, displayModeItems, IM_ARRAYSIZE(displayModeItems)))
			{
				DisplayMode = static_cast<RenderingMode>(displayMode + 1);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Mesh Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			static std::unique_ptr<std::thread> simpliThread(nullptr);
			static std::mutex mutex;
			static bool fredFinished = true;
			static bool loopPassed = false;
			if (fredFinished)
			{
				if (simpliThread)
				{
					(*entity.GetMesh())->GenerateNormals(false);
					simpliThread->join();
					simpliThread.reset();
				}
				ImGui::Checkbox("Auto-Rotation", &autoRotation);
				if (ImGui::Checkbox("Flat Mesh", (bool *)entity.GetShaderAttribute<int>("isNormalFlat")))
				{
					if (*entity.GetShaderAttribute<int>("isNormalFlat")) entity.SetMesh(meshObjNotSmooth);
					else entity.SetMesh(meshObjSmooth);
				}
				if (ImGui::Button("Simplify"))
				{
					fredFinished = false;
					simpliThread.reset(new std::thread([&](Mesh * mesh) {
						mesh->SimplifyParallel(loopPassed, fredFinished, &mutex);
					}, &entity.GetMesh()));
				}
				if (ImGui::Button("Subdivide"))
				{
					entity.GetMesh().Subdivide();
					//fredFinished = false;
					//simpliThread.reset(new std::thread([&](Mesh * mesh) {
					//	mesh->SimplifyParallel(loopPassed, fredFinished, &mutex);
					//	}, &entity.GetMesh()));
				}
			}
			else
			{
				if (loopPassed)
				{
					mutex.lock();
					(*entity.GetMesh())->GenerateNormals(false);
					loopPassed = false;
					mutex.unlock();
				}
			}
			ImGui::LabelText("Vertices", "%d", entity.GetMesh().verticesNVert());
			ImGui::LabelText("Faces", "%d", entity.GetMesh().facesNVert() / 3);

			gui.EditEntity(entity);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Material Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("Shininess", &entityMaterial.shininess, 0.0f, 1024.0f);
			ImGui::ColorEdit3("Diffuse", glm::value_ptr(entityMaterial.diffuseColor));
			ImGui::ColorEdit3("Specular", glm::value_ptr(entityMaterial.specularColor));

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Light Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("Constant", &sun.__constant, 0.0f, 1.0f);
			ImGui::SliderFloat("Linear", &sun.__linear, 0.01f, 1.0f, "%.8f");
			ImGui::SliderFloat("Quadratic", &sun.__quadratic, 0.001f, 1.0f, "%.10f");

			if (ImGui::SliderFloat("Ambient", &sun.__ambient.x, 0.0f, 1.0f))
				sun.__ambient.y = sun.__ambient.z = sun.__ambient.x;
			if (ImGui::SliderFloat("Diffuse", &sun.__diffuse.x, 0.0f, 1.0f))
				sun.__diffuse.y = sun.__diffuse.z = sun.__diffuse.x;
			if (ImGui::SliderFloat("Specular", &sun.__specular.x, 0.0f, 1.0f))
				sun.__specular.y = sun.__specular.z = sun.__specular.x;

			ImGui::TreePop();
		}

		ImGui::End();
		return true;
	});

	float backgroundColor[4] = { 0.0f, 0.0f, 0.03f, 1.0f };

	glPointSize(5.0f);

	AxisDisplayer axisDisplayer(Rendering::Shaders(Constants::Paths::axisDisplayerShaderVertex));

	window->Loop([&]() {
		// Render
		// Clear the colorbuffer
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Switch mesh mode
		if (window->key(GLFW_KEY_C) == InputKey::JustPressed)
			DisplayMode = (RenderingMode)(((int)((DisplayMode % 7) + 1)));

		// Camera Lock
		if (window->key(GLFW_KEY_L) == InputKey::JustPressed)
		{
			cameraLock = !cameraLock;
			glfwSetInputMode(window->window, GLFW_CURSOR,
				cameraLock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}

		// Font change
		if (window->key(GLFW_KEY_F) == InputKey::JustPressed)
		{
			Rendering::RotateFonts();
		}

		if (window->key(GLFW_KEY_LEFT) == InputKey::Pressed)
		{
			if (window->key(GLFW_KEY_LEFT_SHIFT) == InputKey::Pressed)
				sun.pos.z -= window->DeltaTime() * 2.0f;
			else
				sun.pos.x -= window->DeltaTime() * 2.0f;
		}
		if (window->key(GLFW_KEY_RIGHT) == InputKey::Pressed)
		{
			if (window->key(GLFW_KEY_LEFT_SHIFT) == InputKey::Pressed)
				sun.pos.z += window->DeltaTime() * 2.0f;
			else
				sun.pos.x += window->DeltaTime() * 2.0f;
		}
		if (window->key(GLFW_KEY_UP) == InputKey::Pressed)
		{
			sun.pos.y += window->DeltaTime() * 2.0f;
		}
		if (window->key(GLFW_KEY_DOWN) == InputKey::Pressed)
		{
			sun.pos.y -= window->DeltaTime() * 2.0f;
		}

		// Enable/Disable GUI
		if (window->key(GLFW_KEY_T) == InputKey::JustPressed) enableGui = !enableGui;

		if (cameraLock)
		{
			// Camera movement
			if (window->key(GLFW_KEY_W) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Forward, window->DeltaTime());
			if (window->key(GLFW_KEY_S) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Backward, window->DeltaTime());
			if (window->key(GLFW_KEY_A) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Left, window->DeltaTime());
			if (window->key(GLFW_KEY_D) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Right, window->DeltaTime());
			if (window->key(GLFW_KEY_SPACE) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Up, window->DeltaTime());
			if (window->key(GLFW_KEY_LEFT_CONTROL) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Down, window->DeltaTime());

			// Camera Rotation
			if (window->mouseHasMoved())
				camera.ProcessMouseMovement(window->mousePositionOffset().x, window->mousePositionOffset().y);
			if (window->mouseScrollHasMoved())
				camera.ProcessMouseScroll(window->mouseWheelOffset().y);
		}

		// Window Dimensions changed
		if (window->windowDimensionsHasChanged())
			camera.SetWindowDimensions(window->windowWidth(), window->windowHeight());

		// Camera/View transformation
		camera.GetProjViewMatrixUbo();

		// Wireframe Color change
		if (window->key(GLFW_KEY_P) == InputKey::JustPressed)
			Rendering::RotateWireframeColor();

		if (autoRotation)
		{
			entity.quat.RotateY(30.0f * window->DeltaTime());
		}

		Rendering::Refresh();

		// display mode & activate shader
		Rendering::DrawEntity(entity);
		Rendering::DrawEntity(sun);

		if (enableGui)
		{
			axisDisplayer.Draw();
			// Drawing ImGui GUI
			if (!gui.DrawGUI()) return false;
		}

		return true;
	});

	return EXIT_SUCCESS;
}