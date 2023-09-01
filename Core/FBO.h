//
// Created by bios on 8/31/23.
//

#ifndef GAME_FBO_H
#define GAME_FBO_H


#include "../glad/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include "stdexcept"

class FBO {
public:
    FBO(int width, int height);
    ~FBO();

    void Resize(int newWidth, int newHeight);

    void Bind();
    void Unbind();

    GLuint GetTexture() const;

private:
    GLuint framebufferID;
    GLuint textureID;
    int width;
    int height;
};


#endif //GAME_FBO_H
