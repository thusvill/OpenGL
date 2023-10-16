//
// Created by bios on 9/3/23.
//

#ifndef GAME_SceneHierarchyPanel_H
#define GAME_SceneHierarchyPanel_H

#include "../Core.h"
#include "../Scene/Scene.h"
#include "../Scene/Entity.h"

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() =default;
    SceneHierarchyPanel(const Ref<Scene>& contex);
    void SetContex(const Ref<Scene>& contex);

    void OnStart();
    void OnImguiRender();
private:
    template<typename T>
    void DisplayAddComponentEntry(const std::string& entryName);
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
    Ref<Scene> m_Contex;
    Entity m_EntitySelectionContex;
};


#endif //GAME_SceneHierarchyPanel_H
