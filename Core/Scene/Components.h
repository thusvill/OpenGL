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

    bool operator!=(const TagComponent& other) const {
        // Implement the logic to compare two TagComponent objects
        return Tag != other.Tag;
    }
};


struct Transform{
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    Transform() = default;
    Transform(const Transform&) = default;
    Transform(float scale) :Scale(glm::vec3(scale)){};

    bool operator!=(const Transform& other) const {
        return Scale != other.Scale;
    }
};

struct MeshRenderer{
    Model mesh;
    Shader shader;

    MeshRenderer() =  default;
    MeshRenderer(const MeshRenderer&) = default;
    MeshRenderer(Model new_mesh, Shader newShader): mesh(new_mesh), shader(newShader){};

};

struct CameraComponent{
    Camera camera;
    bool Primary = true;

    CameraComponent()=default;
    CameraComponent(const CameraComponent&) =default;
    CameraComponent(Camera cam, bool primary =  true): camera(cam), Primary(primary){};

};

struct LightComponent{
    Shader m_Shader;
    int type;
    glm::vec4 Color;
    glm::vec3 Position;
    float intensity;

    LightComponent()= default;
    LightComponent(const LightComponent&) = default;
    LightComponent(Shader shader, int lightType = 0): m_Shader(shader), type(lightType){};

    bool operator!=(const LightComponent& other) const {
        return m_Shader.ID != other.m_Shader.ID;
    }

};

#endif //GAME_COMPONENTS_HPP
