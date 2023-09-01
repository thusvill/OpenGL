//
// Created by bios on 8/12/23.
//

#ifndef GAME_EBO_H
#define GAME_EBO_H
#include "../glad/include/glad/glad.h"
#include <vector>

class EBO {
public:
    GLuint ID;
    EBO(std::vector <GLuint>& indices);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //GAME_EBO_H
