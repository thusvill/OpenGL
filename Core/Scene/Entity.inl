//
// Created by bios on 9/2/23.
//

#ifndef GAME_ENTITY_INL
#define GAME_ENTITY_INL

// Entity.inl

template<typename T>
bool Entity::HasComponent() {
    return m_Scene->m_Registry.owned<T>(m_EntityHandle);
}

template<typename T, typename ...Args>
T& Entity::AddComponent(Args &&...args) {
    return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
}

template<typename T>
T& Entity::GetComponent() {
    return m_Scene->m_Registry.get<T>(m_EntityHandle);
}

template<typename T>
T& Entity::RemoveComponent() {
    return m_Scene->m_Registry.remove<T>(m_EntityHandle);
}

#endif //GAME_ENTITY_INL
