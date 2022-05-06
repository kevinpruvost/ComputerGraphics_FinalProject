# CMakeList.txt : Top-level CMake project file

cmake_minimum_required (VERSION 3.8)

# Project Name
project ("FinalProject")

# C++ Standard Version
set(CMAKE_CXX_STANDARD 20)

######################################
## C++ Compiling Flags
set(CMAKE_CXX_STANDARD 20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_CXX_FLAGS         "")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")          # Debug + Compile Time Optimization
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")             # Runtime Optimization
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS         "")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")          # Debug + Compile Time Optimization
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")             # Runtime Optimization
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    set(CMAKE_CXX_FLAGS         "")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Od -DEBUG")      # Debug + Compile Time Optimization
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")    # Runtime Optimization
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})

# Library Directory
add_subdirectory("lib")

# Library Directory
add_subdirectory("Documentation")

# Assignment2 Executable
file (GLOB ASSIGNMENT_SOURCES
    *.cpp
    OGL_Implementation/*.cpp
    OGL_Implementation/DebugInfo/*.cpp
    OGL_Implementation/Mesh/*.cpp
    OGL_Implementation/Mesh/Modules/*.cpp
    OGL_Implementation/Shader/*.cpp
    OGL_Implementation/Entity/*.cpp
    OGL_Implementation/EntityAttribute/*.cpp
    OGL_Implementation/Entity/ParticleSystem/*.cpp
    OGL_Implementation/Entity/ParticleSystem/Spirals/*.cpp
    OGL_Implementation/Entity/ParticleSystem/Snow/*.cpp
    OGL_Implementation/Image/*.cpp
    OGL_Implementation/Light/*.cpp
    OGL_Implementation/Rendering/*.cpp
    OGL_Implementation/Text/*.cpp
    OGL_Implementation/Tools/*.cpp
)

file (GLOB ASSIGNMENT_HEADERS
    *.hpp
    OGL_Implementation/*.hpp
    OGL_Implementation/DebugInfo/*.hpp
    OGL_Implementation/Mesh/*.inl
    OGL_Implementation/Mesh/*.hpp
    OGL_Implementation/Mesh/Modules/*.hpp
    OGL_Implementation/Shader/*.hpp
    OGL_Implementation/Entity/*.hpp
    OGL_Implementation/EntityAttribute/*.hpp
    OGL_Implementation/EntityAttribute/*.inl
    OGL_Implementation/Entity/*.inl
    OGL_Implementation/Entity/ParticleSystem/*.hpp
    OGL_Implementation/Entity/ParticleSystem/*.inl
    OGL_Implementation/Entity/ParticleSystem/Spirals/*.hpp
    OGL_Implementation/Entity/ParticleSystem/Snow/*.hpp
    OGL_Implementation/Image/*.hpp
    OGL_Implementation/Light/*.hpp
    OGL_Implementation/Rendering/*.hpp
    OGL_Implementation/Text/*.hpp
    OGL_Implementation/Tools/*.hpp
)

add_executable(FinalProject
    ${ASSIGNMENT_SOURCES}
    ${ASSIGNMENT_HEADERS}
)

# Linking libraries to Assignment2
target_link_libraries(FinalProject opengl32 glad glfw glm ImGui SOIL freetype
)

# Profiling
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")

#--------------------------------------------------------------------
# Hide the console window in visual studio projects - Release
#--------------------------------------------------------------------
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

# Including root directory & libraries directories
target_include_directories(FinalProject PUBLIC
    ${PROJECT_SOURCE_DIR}
    $<INSTALL_INTERFACE:include>
)

# Installation Procedure
install(
    TARGETS FinalProject
    CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
)