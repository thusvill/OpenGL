//
// Created by bios on 9/2/23.
//
#include "Scene.hpp"

Scene::Scene() {
    auto entity = m_Registry.create();
    m_Registry.emplace<Transform>(entity);
    std::cout<<"Scene created"<<std::endl;
}

Scene::~Scene() {

}