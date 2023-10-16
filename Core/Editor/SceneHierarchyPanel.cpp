//
// Created by bios on 9/3/23.
//

#include "SceneHierarchyPanel.h"
#include "../../imgui/imgui.h"
#include "../Scene/Components.h"
#include "../GameEngine.h"

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &contex) {
    SetContex(contex);
}

void SceneHierarchyPanel::SetContex(const Ref<Scene> &contex) {
    m_Contex = contex;
}

void SceneHierarchyPanel::OnStart() {
    //Light
    {
        //auto groupL = m_Registry.group<Transform>(entt::get<LightComponent>);
        auto groupL = m_Contex->m_Registry.view<Transform, LightComponent>();
        for (auto entity:groupL) {
            auto [transform, light] = groupL.get<Transform, LightComponent>(entity);
            light.Color = glm::vec4(1.0f);
            light.intensity = 1.0f;
        }
    }
}

void SceneHierarchyPanel::OnImguiRender() {
    ImGui::Begin("Scene Hierachy");
    if(m_Contex) {
        /*auto list = m_Contex->m_Registry.view<TagComponent>();
        for (auto entityID: list) {
            if (m_Contex->m_Registry.all_of<TagComponent>(entityID)) {
                // The current entity has a TagComponent
                Entity entity(entityID, m_Contex.get());
                DrawEntityNode(entity);

            }
        }*/

        m_Contex->m_Registry.view<TagComponent>().each([&](auto entityID, TagComponent& tag){
            Entity entity{ entityID , m_Contex.get() };
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_EntitySelectionContex = {};
        }

        //TODO: develop this Popups
        if (ImGui::IsMouseDoubleClicked(1) && ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                m_Contex->CreateEntity("Empty Entity");

            }
            ImGui::EndPopup();
        }


    }
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_EntitySelectionContex) {
            DrawComponents(m_EntitySelectionContex);
            if(ImGui::Button("Add Component")){
                ImGui::OpenPopup("Add Component");
            }
        }
        if(ImGui::BeginPopup("Add Component")){
            if(ImGui::MenuItem("Light")){
                //m_EntitySelectionContex.AddComponent<LightComponent>();
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::MenuItem("Mesh Renderer")){
                m_EntitySelectionContex.AddComponent<MeshRenderer>();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();


}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ((m_EntitySelectionContex == entity) ? ImGuiTreeNodeFlags_Selected: 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if(ImGui::IsItemClicked()) {
        m_EntitySelectionContex = entity;
    }


    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Create Empty Entity")) {
            m_Contex->CreateEntity("Empty Entity");
        }
        if (ImGui::MenuItem("Delete Entity")) {
            entityDeleted = true;
        }
        ImGui::EndPopup();
    }

    if(opened)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened)
            ImGui::TreePop();
        ImGui::TreePop();
    }
    if(entityDeleted){
        m_Contex->DestroyEntity(entity);
        if(m_EntitySelectionContex == entity){
            entity= {};
        }
    }

}


char vertex_S[256] = "Core/Shaders/default.vert";
char fragment_S[256] = "Core/Shaders/default.frag";
char geometry_S[256] = "Core/Shaders/default.geom";
bool pathCopied = false;
char m_path[512];
void SceneHierarchyPanel::DrawComponents(Entity entity) {

    if(entity.HasComponent<TagComponent>()){
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, tag.c_str());
        if(ImGui::InputText("Tag", buffer, sizeof(buffer))){
            tag = std::string(buffer);
        }
    }
    if(entity.HasComponent<Transform>()){

        if(ImGui::TreeNodeEx((void*) typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            DrawVec3Controls("Position", entity.GetComponent<Transform>().Position);
            DrawVec3Controls("Rotation", entity.GetComponent<Transform>().Rotation);
            DrawVec3Controls("Scale", entity.GetComponent<Transform>().Scale, 1.0f);

            ImGui::TreePop();
        }
    }

    if(entity.HasComponent<LightComponent>()){

        if(ImGui::TreeNodeEx((void*) typeid(LightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Light")) {
            auto shader = entity.GetComponent<LightComponent>().m_Shader;
            auto light = entity.GetComponent<LightComponent>();

            const char *lightTypes[] = {"Directional", "Point", "Spot"};

            if (ImGui::Combo("Light Type", &selectedLightType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
                light.type = selectedLightType;
            }

            ImGui::ColorEdit4("Light Color", glm::value_ptr(light.Color));
            ImGui::DragFloat("Intensity", &light.intensity, 0.5f);

            //ImGui::DragFloat3("Position", glm::value_ptr(lightPos), 0.5f, -10.0f, 10.0f);
            DrawVec3Controls("Light Position",entity.GetComponent<Transform>().Position);
            //UpdateLight(light.Color, light.Position, shader);
            entity.GetComponent<LightComponent>() = light;

            ImGui::TreePop();
        }
    }
    if(entity.HasComponent<CameraComponent>())
    {

        if(ImGui::TreeNodeEx((void*) typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
            auto camera = entity.GetComponent<CameraComponent>();
            auto transform = entity.GetComponent<Transform>();
            if (ImGui::Button("Reset to preset")) {
                camera.camera.Position = glm::vec3(2.07479f, 1.249f, -0.381916f);
                camera.camera.Orientation = glm::vec3(-4.71458f, -1.41498f, 0.173473f);
            }
            ImGui::Separator();
            const char *cameraType[] = {"Perspective", "Orthographic"};
            if (ImGui::Combo("Camera type", &camType, cameraType, IM_ARRAYSIZE(cameraType))) {
                switch (camType) {
                    case 0:
                        camera.camera.SetCameraMode(Camera::CamMode::Perspective);
                        break;
                    case 1:
                        camera.camera.SetCameraMode(Camera::CamMode::Orthographic);
                        break;

                }
            }
            ImGui::SeparatorText("View");

            if (camera.camera.mode == Camera::CamMode::Perspective) {
                ImGui::DragFloat("FOV", &camera.camera.fov, 1.0f, 5.0f, 180.0f);
            }

            ImGui::DragFloat("Near Plane", &camera.camera.nearPlane, 0.1f, 0.1f, 100.0f);
            ImGui::DragFloat("Far Plane", &camera.camera.farPlane, 1.0f, 1.0f, 1000.0f);

            ImGui::SeparatorText("Controls");
            ImGui::DragFloat("Speed", &camera.camera.speed, 0.01f, 0.01f, 5.0f);
            ImGui::DragFloat("Sensitivity", &camera.camera.sensitivity, 0.5f, 1.0f, 100.0f);

            entity.GetComponent<CameraComponent>() = camera;
            entity.GetComponent<Transform>() = transform;

            ImGui::TreePop();
        }
    }
    if(entity.HasComponent<MeshRenderer>()){

        if (!pathCopied && strlen(m_path) != 0) {
            if(entity.GetComponent<MeshRenderer>().mesh.file != NULL) {
                strcpy(m_path, entity.GetComponent<MeshRenderer>().mesh.file);
                pathCopied = true;
            } else{
                strncpy(m_path, "/home/bios/CLionProjects/Game/Models/map/scene.gltf", sizeof(m_path) - 1);
                m_path[sizeof(m_path) - 1] = '\0';
                pathCopied = true;
            }
        }
        if(strlen(m_path) <= 0){
            strcpy(m_path, entity.GetComponent<MeshRenderer>().mesh.file);
        }
        if(ImGui::TreeNodeEx((void*) typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Mesh Renderer")) {
            MeshRenderer meshRenderer = entity.GetComponent<MeshRenderer>();
            ImGui::InputText("Model Path", m_path, sizeof(m_path));
            if(ImGui::Button("Load Model")){
                Model newModel(m_path);
                meshRenderer.mesh = newModel ;
                entity.GetComponent<MeshRenderer>() = meshRenderer;
                std::cout<<"--Loaded Mesh"<<std::endl;

            }

            ImGui::SeparatorText("Shader");
            ImGui::InputText("Vertex", vertex_S, sizeof(vertex_S));
            ImGui::InputText("Fragment", fragment_S, sizeof(fragment_S));
            ImGui::InputText("Geometry", geometry_S, sizeof(geometry_S));
            if(ImGui::Button("Load Shaders")){
                Shader newShader(vertex_S,fragment_S,geometry_S);
                meshRenderer.shader = newShader;
                entity.GetComponent<MeshRenderer>() = meshRenderer;
                auto groupL = m_Contex->m_Registry.view<Transform, LightComponent>();
                for (auto entity: groupL) {
                    auto [transform, light] = groupL.get<Transform, LightComponent>(entity);
                    light.m_Shader= newShader;


                }
                std::cout<<"--Loaded Shader"<<std::endl;

            }

            ImGui::TreePop();
        }
    }
}

template<typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
    if (!m_EntitySelectionContex.HasComponent<T>())
    {
        if (ImGui::MenuItem(entryName.c_str()))
        {
            m_EntitySelectionContex.AddComponent<T>();
            ImGui::CloseCurrentPopup();
        }
    }
}

