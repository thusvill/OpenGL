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

int selectedLightType;
int cullType;
int camType=0;
int cullFace;


glm::vec3 model_position = glm::vec3(0.0f);
glm::vec3 model_rotation = glm::vec3 (0.0f);


#endif //GAME_GAMEENGINE_H
