//
// Created by bios on 9/2/23.
//

#ifndef GAME_COMPONENTS_HPP
#define GAME_COMPONENTS_HPP
#include "../GameEngine.h"


struct TagComponent{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag):Tag(tag){};
};

struct Transform{
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    Transform() = default;
    Transform(const Transform&) = default;
};

struct MeshRenderer{
    Model mesh;
    Shader shader;

    MeshRenderer() =  default;
    MeshRenderer(const MeshRenderer&) = default;
    MeshRenderer(Model new_mesh, Shader newShader): mesh(new_mesh), shader(newShader){};

};

#endif //GAME_COMPONENTS_HPP
