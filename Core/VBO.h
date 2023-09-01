//
// Created by bios on 8/12/23.
//

#ifndef GAME_VBO_H
#define GAME_VBO_H
#include "../glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};


class VBO {
public:
    GLuint ID;
    VBO(std::vector <Vertex>& vertices);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //GAME_VBO_H
