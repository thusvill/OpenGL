//
// Created by bios on 9/2/23.
//
#include "Scene.h"

#include "Components.h"

Scene::Scene(Camera camera): m_currentCamera(camera) {
    auto entity = m_Registry.create();
    m_Registry.emplace<Transform>(entity);
    std::cout<<"Scene created"<<std::endl;
}

Scene::~Scene() {

}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity  = {m_Registry.create(), this};
    entity.AddComponent<Transform>(1.0f);
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;

    return entity;
}
Camera* mainCamera;
void Scene::OnDraw() {



    //Renderings

        auto group = m_Registry.group<Transform>(entt::get<MeshRenderer>);
        for (auto entity: group) {
            //MeshRenderer &meshRenderer = group.get<MeshRenderer>(entity);
            //Transform &transform = group.get<Transform>(entity);
            auto [transform, meshRenderer] = group.get<Transform, MeshRenderer>(entity);
            meshRenderer.mesh.Draw(meshRenderer.shader, m_currentCamera);
        }

}

void Scene::OnUpdate() {

    //Camera
    {
        //auto camgroup = m_Registry.group<Transform>(entt::get<CameraComponent>);
        auto camgroup = m_Registry.view<Transform, CameraComponent>();
        for (auto entity:camgroup) {
            auto [transform, camera] = camgroup.get<Transform, CameraComponent>(entity);
            if(camera.Primary) {
                mainCamera = &camera.camera;
                //apply transforms
                {
                    auto camTrans = camera.camera;
                    camTrans.Position = transform.Position;
                    camTrans.Orientation = transform.Rotation;
                }
                //update matrix
                {
                    camera.camera.updateMatrix();
                }
            }
        }
    }

    auto group = m_Registry.group<Transform>(entt::get<MeshRenderer>);
    for (auto entity: group) {
        auto [transform, mesh] = group.get<Transform, MeshRenderer>(entity);
        //mesh.mesh.SetTransform(mesh.shader, transform.Position, transform.Rotation, transform.Scale);
        {
            mesh.mesh.Position(mesh.shader, transform.Position);
            mesh.mesh.Rotation(mesh.shader, transform.Rotation);
            mesh.mesh.Scale(mesh.shader, transform.Scale);
        }
    }
}