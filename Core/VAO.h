//
// Created by bios on 8/14/23.
//

#ifndef GAME_VAO_H
#define GAME_VAO_H
#include "../glad/include/glad/glad.h"
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO& VBO, GLuint layout,GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};


#endif //GAME_VAO_H
