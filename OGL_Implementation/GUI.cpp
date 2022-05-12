/*****************************************************************//**
 * \file   GUI.cpp
 * \brief  GUI source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "GUI.hpp"

GUI::GUI(GLFWwindow * window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

GUI::~GUI()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

bool GUI::DrawGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize({300.0f, 250.0f}); 
    ImGui::SetNextWindowPos({ 20.0f, 20.0f }, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("Read me !");
    ImGui::Text(
        "Hello Prof/TA !\n"
        "Here are the controls:\n"
        "- Camera/Cursor Lock: L\n"
        "- Camera Movement (has to be unlocked):\n"
        "\t- Forward:  W\n"
        "\t- Backward: S\n"
        "\t- Left:     A\n"
        "\t- Right:    D\n"
        "\t- Up:       Space\n"
        "\t- Down:     Left Ctrl\n"
        "- Camera Rotation: Mouse cursor\n"
        "- Wireframe Color Change: P\n"
        "- Move Light (Sun):\n"
        "\t- X: Left / Right Arrows\n"
        "\t- Y: Down / Up Arrows\n"
        "\t- Z: LSHIFT + (Left / Right) Arrows\n"
    );
    ImGui::End();

    for (auto & callback : __callbacks)
        if (!callback()) return false;

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return true;
}

void GUI::AddCallback(const std::function<bool()> & lambda)
{
    __callbacks.push_back(lambda);
}

void GUI::EditEntity(Entity & entity)
{
    if (ImGui::TreeNodeEx(std::format("{0}", entity.name).c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
    {
        glm::vec3 eulerAngles = entity.quat.ToEulerAngles();
        ImGui::DragFloat3("Position", glm::value_ptr(entity.pos), 0.02f);
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerAngles), 0.5f))
        {
            eulerAngles.x = std::fmod(eulerAngles.x, 360.0f);
            eulerAngles.y = std::fmod(eulerAngles.y, 360.0f);
            eulerAngles.z = std::fmod(eulerAngles.z, 360.0f);
            entity.quat.SetRotation(eulerAngles);
        }
        ImGui::DragFloat3("Scale", glm::value_ptr(entity.scale), 0.02f);
        if (ImGui::TreeNodeEx("Mesh Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (entity.GetMesh().IsMeshOperationFinished())
            {
                if (ImGui::Checkbox("Flat Mesh", (bool *)entity.GetShaderAttribute<int>("isNormalFlat")))
                {
                    if (*entity.GetShaderAttribute<int>("isNormalFlat")) (*entity.GetMesh())->GenerateNormals(true);
                    else (*entity.GetMesh())->GenerateNormals(false);
                }
                if (ImGui::Button("Simplify"))
                {
                    entity.GetMesh().SimplifyParallel();
                }
                ImGui::SameLine();
                if (ImGui::Button("Subdivide"))
                {
                    entity.GetMesh().SubdivideParallel();
                }
            }
            else
            {
                
            }
            ImGui::LabelText("Vertices", "%d", entity.GetMesh().verticesNVert());
            ImGui::LabelText("Faces", "%d", entity.GetMesh().facesNVert() / 3);
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Material Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat("Shininess", &entity.GetMaterial()->shininess, 0.0f, 1024.0f);
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(entity.GetMaterial()->diffuseColor));
            ImGui::ColorEdit3("Specular", glm::value_ptr(entity.GetMaterial()->specularColor));

            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}
