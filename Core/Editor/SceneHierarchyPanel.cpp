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
    auto list = m_Contex->m_Registry.view<TagComponent>();
    for (auto entityID:list) {
        if (m_Contex->m_Registry.all_of<TagComponent>(entityID)) {
            // The current entity has a TagComponent
            Entity entity(entityID, m_Contex.get());
            DrawEntityNode(entity);

        }
    }

    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
        m_EntitySelectionContex = {};
    }

    ImGui::End();

    ImGui::Begin("Properties");
    if(m_EntitySelectionContex){
        DrawComponents(m_EntitySelectionContex);
    }
    ImGui::End();

}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ((m_EntitySelectionContex == entity) ? ImGuiTreeNodeFlags_Selected: 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if(ImGui::IsItemClicked()){
        m_EntitySelectionContex = entity;
    }
    if(opened)
    {
        ImGui::TreePop();
    }

}

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
}