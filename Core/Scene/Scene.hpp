//
// Created by bios on 9/2/23.
//

#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../entt/entt.hpp"
#include "Components.hpp"

class Scene{
public:
    Scene();
    ~Scene();

private:
    entt::registry m_Registry;
    friend class Entity;
};

#endif //GAME_SCENE_HPP
