//
// Created by bios on 9/2/23.
//

#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../entt/entt.hpp"
#include "../Camera.h"

class Entity;
class Camera;
class Scene{
public:
    Scene(Camera camera);
    ~Scene();

    Camera m_currentCamera;

    Entity CreateEntity(const std::string& name = "");

    void OnDraw();
    void OnUpdate();

private:
    entt::registry m_Registry;
    friend class Entity;
    friend class SceneHierarchyPanel;
};

#endif //GAME_SCENE_HPP
