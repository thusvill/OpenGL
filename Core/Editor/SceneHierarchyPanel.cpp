//
// Created by bios on 9/3/23.
//

#include "SceneHierarchyPanel.h"
#include "../../imgui/imgui.h"
#include "../Scene/Components.h"

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &contex) {
    SetContex(contex);
}

void SceneHierarchyPanel::SetContex(const Ref<Scene> &contex) {
    m_Contex = contex;
}
void SceneHierarchyPanel::OnImguiRender() {
    ImGui::Begin("Scene Hierachy");

    /*m_Contex->m_Registry.view<Entity>().each([&](auto entityID) {

            Entity entity(entityID, m_Contex.get());
            auto& tc = entity.GetComponent<TagComponent>();
            ImGui::Text("%s", tc.Tag.c_str());

    });
     */

    auto list = m_Contex->m_Registry.view<TagComponent>();
    for (auto entityID:list) {
        if (m_Contex->m_Registry.all_of<TagComponent>(entityID)) {
            // The current entity has a TagComponent
            Entity entity(entityID, m_Contex.get());
            DrawEntityNode(entity);

        }
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