//
// Created by bios on 9/2/23.
//
#include "Scene.h"

#include "Components.h"

Scene::Scene() {
    auto entity = m_Registry.create();
    m_Registry.emplace<Transform>(entity);
    std::cout<<"Scene created"<<std::endl;
}

Scene::~Scene() {

}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity  = {m_Registry.create(), this};
    entity.AddComponent<Transform>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;

    return entity;
}

void Scene::OnUpdate() {

}