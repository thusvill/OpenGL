//
// Created by bios on 8/12/23.
//

#ifndef GAME_SHADERCLASS_H
#define GAME_SHADERCLASS_H

#include "../glad/include/glad/glad.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <GLFW/glfw3.h>

std::string get_file_contents(const char* filename);
class Shader{
public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

    void Activate();
    void Delete();
private:
    void compileErrors(unsigned int shader, const char* type);
};

#endif // GAME_SHADERCLASS_H
