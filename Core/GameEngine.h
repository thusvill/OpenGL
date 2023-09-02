//
// Created by bios on 9/1/23.
//

#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_internal.h"

#include "Model.h"
#include "FBO.h"
#include "Scene/Scene.hpp"
#include "Core.h"
#include "Scene/Entity.hpp"

extern int selectedLightType;
extern int cullType;
extern int camType;
extern int cullFace;
extern char path[1024];
extern glm::vec3 model_position;
extern glm::vec3 model_rotation;
extern glm::vec3 model_scale;
extern float light_intensity;



    static void DrawVec3Controls(const std::string &label, glm::vec3 &values, float resetValues = 0.0f,
                                 float columnWidth = 100.0f) {

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.79, 0.00, 0.00, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1.00, 0.00, 0.00, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.79, 0.00, 0.00, 1.0});
        if (ImGui::Button("X", buttonSize)) {
            values.x = resetValues;
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.08, 0.72, 0.00, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.00, 1.00, 0.00, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.08, 0.72, 0.00, 1.0});
        if (ImGui::Button("Y", buttonSize)) {
            values.y = resetValues;
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.00, 0.01, 0.72, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.00, 0.01, 1.00, 1.0});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.00, 0.01, 0.72, 1.0});
        if (ImGui::Button("Z", buttonSize)) {
            values.z = resetValues;
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::PopID();

        ImGui::Columns(1);
    }




#endif //GAME_GAM}EENGINE_H
