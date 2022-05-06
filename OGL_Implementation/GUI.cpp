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
    glm::vec3 eulerAngles = entity.quat.ToEulerAngles();
    glm::vec3 beforeEA = eulerAngles;
    ImGui::SliderFloat3("Position", glm::value_ptr(entity.pos), -2.0f, 2.0f);
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(eulerAngles), 0.0f, 359.999f))
    {
        entity.quat.SetRotation(eulerAngles);
//        entity.quat.Rotate(eulerAngles - beforeEA);
    }
}
