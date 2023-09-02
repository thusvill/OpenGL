//
// Created by bios on 9/2/23.
//
#pragma once

#ifndef GAME_ENTITY_HPP
#define GAME_ENTITY_HPP
#include "../GameEngine.h"

class Scene;
class Entity {
public:
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T>
    bool HasComponent();

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);


private:
    entt::entity m_EntityHandle;
    Scene* m_Scene;
};


#endif //GAME_ENTITY_HPP
