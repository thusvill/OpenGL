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
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }


    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);

    template<typename T>
    T& GetComponent();

    template<typename T>
    T& RemoveComponent();

    operator entt::entity() const { return m_EntityHandle;}
    operator uint32_t() const { return (uint32_t)m_EntityHandle;}
    bool operator==(const Entity& other) const
    {
        return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }
    bool operator!=(const Entity& other) const
    {
        return (*this == other);
    }
private:
    entt::entity m_EntityHandle{ 0 };
    Scene* m_Scene = nullptr;
};

#include "Entity.inl"

#endif //GAME_ENTITY_HPP
