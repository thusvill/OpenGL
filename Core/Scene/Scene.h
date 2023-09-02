//
// Created by bios on 9/2/23.
//

#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../entt/entt.hpp"

class Entity;
class Scene{
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = "");

    void OnUpdate();

private:
    entt::registry m_Registry;
    friend class Entity;
};

#endif //GAME_SCENE_HPP
