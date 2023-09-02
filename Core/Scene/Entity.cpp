//
// Created by bios on 9/2/23.
//

#include "Entity.h"


    Entity::Entity(entt::entity handle, Scene *scene)
            : m_Scene(scene), m_EntityHandle(handle) {
        std::cout << "Entity Created!" << std::endl;
    }

