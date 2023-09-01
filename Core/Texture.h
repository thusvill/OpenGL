//
// Created by bios on 8/17/23.
//

#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H
#include "../glad/include/glad/glad.h"
#include "stb/stb_image.h"

#include "shaderClass.h"

class Texture {
public:
    GLuint ID;
    const char*type;
    GLuint unit;
    Texture(const char* image, const char* texType, GLuint slot);

    void texUni(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};


#endif //GAME_TEXTURE_H
