//
// Created by bios on 9/2/23.
//
#pragma once

#ifndef GAME_ENTITY_HPP
#define GAME_ENTITY_HPP
#include "../GameEngine.h"
#include "Scene.hpp"


class Entity {
public:
    Entity();
    Entity(const Entity& other) = default;



private:
    entt::entity m_EntityHandle;
    Scene* m_Scene;
};


#endif //GAME_ENTITY_HPP
