# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#

project(ImGui)

cmake_minimum_required (VERSION 3.8)

add_library (ImGui STATIC
"imgui.cpp"
"imgui_demo.cpp"
"imgui_draw.cpp"
"imgui_impl_glfw.cpp"
"imgui_impl_opengl3.cpp"
"imgui_tables.cpp"
"imgui_widgets.cpp"

"include/imconfig.h"
"include/imgui.h"
"include/imgui_impl_glfw.h"
"include/imgui_impl_opengl3.h"
"include/imgui_internal.h" 
"include/imstb_rectpack.h"
"include/imstb_textedit.h"
"include/imstb_truetype.h"
)

target_include_directories(ImGui PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(ImGui opengl32 glad glfw glm)

export(PACKAGE ImGui)

# TODO: Add tests and install targets if needed.
