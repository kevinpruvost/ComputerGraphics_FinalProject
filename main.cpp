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

#include "OGL_Implementation\Cubemap\Brdf_Cubemap.hpp"

#include "Constants.hpp"

#include <thread>

int main()
{
	Window * window = Window::Init(Constants::Window::windowName, Constants::Paths::windowIcon);
	if (!window)
		return EXIT_FAILURE;

	Rendering::Init();

	Brdf_Cubemap cubemap(Constants::Paths::Textures::Cubemap::texture, Rendering::Shaders(Constants::Paths::backgroundVertex));

	Texture sunTexture;
	if (!sunTexture.GenerateTexture("resources/Textures/sun.jpg"))
	{
		LOG_PRINT(stderr, "Couldn't load texture '%s'\n", "resources/Textures/sun.jpg");
		return EXIT_FAILURE;
	}
	Obj obj4;
	if (!obj4.TryLoad(Constants::Paths::Models::Face::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Face::objFile);
		return EXIT_FAILURE;
	}
	Obj obj1;
	if (!obj1.TryLoad(Constants::Paths::Models::Bunny::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Bunny::objFile);
		return EXIT_FAILURE;
	}
	Obj obj2;
	if (!obj2.TryLoad(Constants::Paths::Models::Cube::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Cube::objFile);
		return EXIT_FAILURE;
	}
	Obj obj3;
	if (!obj3.TryLoad(Constants::Paths::Models::Icosahedron::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Icosahedron::objFile);
		return EXIT_FAILURE;
	}
	Obj objhumanface2;
	if (!objhumanface2.TryLoad(Constants::Paths::Models::Face2::objFile))
	{
		LOG_PRINT(stderr, "Couldn't load obj '%s'\n", Constants::Paths::Models::Face2::objFile);
		return EXIT_FAILURE;
	}
	obj1.GenerateNormals(true);
	Mesh meshObjSmooth = GenerateMesh(obj1.verticesPos, obj1.verticesNormals, obj1.faces);
	obj2.GenerateNormals(true);
	obj3.GenerateNormals(true);
	Mesh mesh2 = GenerateMesh(obj2.verticesPos, obj2.verticesNormals, obj2.faces);
	Mesh mesh3 = GenerateMesh(obj3.verticesPos, obj3.verticesNormals, obj3.faces);
	Mesh mesh4 = GenerateMesh(obj4);
	Mesh meshCube = GenerateMesh(obj2);

	Entity entity1(meshObjSmooth,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::faceShaderVertex));
	Entity entity2(mesh2,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::faceShaderVertex));
	Entity entity3(mesh3,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::faceShaderVertex));
	Entity humanHead(mesh4,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::pbrVertex));
	Entity plane(meshCube,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::pbrVertex));
	entity1.name = "Bunny";
	entity2.name = "Cube";
	entity3.name = "Icosahedron";
	plane.name = "Ground";
	humanHead.name = "HumanFace";
	Material & entityMaterial = entity1.AddMaterial();
	Material & entity2Material = entity2.AddMaterial();
	Material & entity3Material = entity3.AddMaterial();
	entity1.SetShaderAttribute("isNormalFlat", 0);
	entity2.SetShaderAttribute("isNormalFlat", 1);
	entity3.SetShaderAttribute("isNormalFlat", 1);
	humanHead.SetShaderAttribute("isNormalFlat", 0);
	plane.SetShaderAttribute("isNormalFlat", 0);
	entityMaterial.diffuseColor = entity2Material.diffuseColor = entity3Material.diffuseColor = glm::vec3(1.0f);
	entityMaterial.specularColor = entity2Material.specularColor = entity3Material.specularColor = glm::vec3(0.0f);

	plane.pos = glm::vec3(-4.0f, -6.0f, -6.0f );
	plane.scale = glm::vec3(15.0f, 1.0f, 15.0f);
	entity1.scale = glm::vec3(10.0f);
	entity1.pos = glm::vec3(-1.0f, 0.0f, -1.0f);
	entity3.pos = glm::vec3(1.5f, 0.0f, 1.5f);
	humanHead.scale = glm::vec3(0.3f);
	humanHead.pos = glm::vec3(0.0f, 0.0f, -9.0f);

	humanHead.AddPbrMaterial(
		Constants::Paths::Textures::HumanHead::albedo,
		Constants::Paths::Textures::HumanHead::normal,
		Constants::Paths::Textures::HumanHead::metallic,
		Constants::Paths::Textures::HumanHead::roughness,
		Constants::Paths::Textures::HumanHead::ao
	);
	humanHead.SetShaderAttribute("translucency", 0.85f);
	humanHead.SetShaderAttribute("sssWidth", 85.0f);
	humanHead.SetShaderAttribute("ssssEnabled", 0);

	Mesh sphereMesh = GenerateMeshSphere();
	PointLight sun(sphereMesh);
	sun.focus = &humanHead;
	sun.SetTexture(sunTexture);
	sun.pos = { -3.1f, 1.46f, -11.8f };
	sun.ChangeSpecular(glm::vec3(1.0f));
	sun.ChangeDiffuse(glm::vec3(5.0f));
	sun.ChangeAmbient(glm::vec3(1.0f));
	sun.name = "Sun";

	Entity goldBall(sphereMesh,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::pbrVertex));
	goldBall.SetShaderAttribute("isNormalFlat", 0);
	goldBall.AddPbrMaterial(
		Constants::Paths::Textures::Gold::albedo,
		Constants::Paths::Textures::Gold::normal,
		Constants::Paths::Textures::Gold::metallic,
		Constants::Paths::Textures::Gold::roughness,
		Constants::Paths::Textures::Gold::ao
	);
	plane.AddPbrMaterial(
		Constants::Paths::Textures::Gold::albedo,
		Constants::Paths::Textures::Gold::normal,
		Constants::Paths::Textures::Gold::metallic,
		Constants::Paths::Textures::Gold::roughness,
		Constants::Paths::Textures::Gold::ao
	);
	goldBall.scale = glm::vec3(2.5f);
	goldBall.pos = glm::vec3(-9.0f, 1.5f, -1.0f);

	Camera camera(window->windowWidth(), window->windowHeight(), -2.0f, 4.0f, 5.0f);
	camera.MovementSpeed *= 1.0f;
	mainCamera = &camera;

	camera.LookAt(humanHead.pos);

	Mesh meshface2 = GenerateMesh(objhumanface2);
	Entity humanHead2(meshface2,
		Rendering::Shaders(Constants::Paths::pointShaderVertex),
		Rendering::Shaders(Constants::Paths::wireframeShaderVertex),
		Rendering::Shaders(Constants::Paths::pbrVertex));
	humanHead2.name = "HumanFace2";
	humanHead2.SetShaderAttribute("isNormalFlat", 0);
	humanHead2.scale = glm::vec3(12.0f);
	humanHead2.pos = glm::vec3(-6.0f, 3.0f, -8.0f);
	humanHead2.AddPbrMaterial(
		Constants::Paths::Textures::HumanHead2::albedo,
		Constants::Paths::Textures::HumanHead2::normal,
		Constants::Paths::Textures::HumanHead2::metallic,
		Constants::Paths::Textures::HumanHead2::roughness,
		Constants::Paths::Textures::HumanHead2::ao
	);
	humanHead2.SetShaderAttribute("translucency", 0.85f);
	humanHead2.SetShaderAttribute("sssWidth", 85.0f);
	humanHead2.SetShaderAttribute("ssssEnabled", 0);

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

			if (ImGui::Checkbox("SSSS Enabled", (bool *)humanHead.GetShaderAttribute<int>("ssssEnabled")))
			{
				*humanHead2.GetShaderAttribute<int>("ssssEnabled") = *humanHead.GetShaderAttribute<int>("ssssEnabled");
			}
			if (ImGui::SliderFloat("SSS Width", humanHead.GetShaderAttribute<float>("sssWidth"), 0.0f, 100.025f))
			{
				*humanHead2.GetShaderAttribute<float>("sssWidth") = *humanHead.GetShaderAttribute<float>("sssWidth");
			}
			if (ImGui::SliderFloat("Translucency", humanHead.GetShaderAttribute<float>("translucency"), 0.0f, 1.0f))
			{
				*humanHead2.GetShaderAttribute<float>("translucency") = *humanHead.GetShaderAttribute<float>("translucency");
			}
			int displayMode = DisplayMode;
			bool verticesDisplay   = (displayMode) & RenderingMode::VerticesMode;
			bool wireframesDisplay = (displayMode) & RenderingMode::WireframeMode;
			bool facesDisplay      = (displayMode) & RenderingMode::FacesMode;
			ImGui::Checkbox("Display Vertices",   &verticesDisplay);
			ImGui::Checkbox("Display Wireframes", &wireframesDisplay);
			ImGui::Checkbox("Display Faces",      &facesDisplay);
			DisplayMode = static_cast<RenderingMode>(verticesDisplay * RenderingMode::VerticesMode + wireframesDisplay * RenderingMode::WireframeMode + facesDisplay * RenderingMode::FacesMode);

			ImGui::Checkbox("Auto-Rotation", &autoRotation);
			ImGui::TreePop();
		}

		auto entities = Entity::GetAllEntities();
		if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto & entity : entities)
			{
				gui.EditEntity(*entity);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Light Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", glm::value_ptr(sun.pos));
			std::vector<const char *> entityNames;
			entityNames.reserve(entities.size());
			int entityNum = 0;
			for (int x = 0; x < entities.size(); ++x)
			{
				if (entities[x] == sun.focus)
					entityNum = x;
				entityNames.push_back(entities[x]->name.c_str());
			}
			if (ImGui::BeginCombo("Light Focus", entityNames[entityNum]))
			{
				for (int n = 0; n < entityNames.size(); n++)
				{
					const bool is_selected = (entityNum == n);
					if (ImGui::Selectable(entityNames[n], is_selected))
					{
						entityNum = n;
						sun.focus = entities[n];
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Constant", &sun.__constant, 0.0f, 1.0f);
			ImGui::SliderFloat("Linear", &sun.__linear, 0.01f, 1.0f, "%.8f");
			ImGui::SliderFloat("Quadratic", &sun.__quadratic, 0.001f, 1.0f, "%.10f");

			if (ImGui::SliderFloat("Ambient", &sun.__ambient.x, 0.0f, 1.0f))
				sun.__ambient.y = sun.__ambient.z = sun.__ambient.x;
			if (ImGui::SliderFloat("Diffuse", &sun.__diffuse.x, 0.0f, 100.0f))
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
			entity1.quat.RotateY(30.0f * window->DeltaTime());
			entity2.quat.RotateY(30.0f * window->DeltaTime());
			entity3.quat.RotateY(30.0f * window->DeltaTime());
			humanHead.quat.RotateY(30.0f * window->DeltaTime());
		}

		Rendering::Refresh();

		// display mode & activate shader
		Rendering::DrawBrdfCubemap(cubemap);
		Rendering::DrawEntity(entity1);
		Rendering::DrawEntity(entity2);
		Rendering::DrawEntity(entity3);
		Rendering::DrawEntity(plane);
		Rendering::DrawEntity(humanHead);
		Rendering::DrawEntity(humanHead2);
		Rendering::DrawEntity(goldBall);
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