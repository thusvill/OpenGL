//
// Created by bios on 9/2/23.
//

#include "Entity.hpp"


    Entity::Entity(entt::entity handle, Scene *scene)
            : m_Scene(scene), m_EntityHandle(handle) {
        std::cout << "Entity Created!" << std::endl;
    }

template<typename T>
bool Entity::HasComponent() {
    return m_Scene->m_Registry.owned<T>(m_EntityHandle);
}

template<typename T, typename ...Args>
T &Entity::AddComponent(Args &&...args) {
    return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
}
