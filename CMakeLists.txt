cmake_minimum_required(VERSION 3.0)
project(Game)

# Set C++ standard for the target
set(CMAKE_CXX_STANDARD 17)

# Add the executable target
file(GLOB_RECURSE SRC_FILES "Core/*.cpp")
add_executable(Game ${SRC_FILES} main.cpp Core/shaderClass.cpp Core/VBO.cpp Core/VBO.h Core/EBO.cpp Core/EBO.h Core/VAO.cpp Core/VAO.h Core/stb/stb_image.cpp Core/stb/stb_image.h Core/Texture.cpp Core/Texture.h Core/Camera.cpp Core/Camera.h Core/Mesh.cpp Core/Mesh.h Core/Mesh.cpp Core/json/json.h Core/Model.cpp Core/Model.h Core/FBO.cpp Core/FBO.h Core/GameEngine.h Core/entt/entt.hpp Core/Core.h Core/Scene/Scene.cpp Core/Scene/Scene.h Core/Scene/Entity.cpp Core/Scene/Entity.h Core/Scene/Entity.inl Core/Scene/Components.h Core/Editor/SceneHierarchyPanel.cpp Core/Editor/SceneHierarchyPanel.h)

# Find the OpenGL package
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

# Add GLAD source files to the project
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# Find the GLFW package
find_package(glfw3 REQUIRED)

# Find the GLM package
find_package(glm REQUIRED)

# Add the ImGui source files to your project
file(GLOB IMGUI_SOURCES "imgui/*.cpp")

# Add the ImGui library as a target
add_library(imgui STATIC ${IMGUI_SOURCES})


# Link against the necessary libraries, including ImGui
target_link_libraries(Game PRIVATE OpenGL::GL glad GLEW glfw glm::glm imgui)

# Include directories for the project
target_include_directories(Game PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(Game PRIVATE ${CMAKE_SOURCE_DIR}/imgui)  # Include ImGui directory
