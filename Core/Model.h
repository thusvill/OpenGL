//
// Created by bios on 8/19/23.
//

#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "json/json.h"
#include "Mesh.h"

using json = nlohmann::json;


class Model {
public:
    Model(const char* file);
    void Draw(Shader& shader, Camera& camera);
    void Position(Shader shader, glm::vec3 newPosition);
    void Rotation(Shader shader, glm::vec3 rotation);
    void Scale(Shader shader, glm::vec3 newScale);
    void SetTransform(Shader shader, glm::vec3 position,glm::vec3 rotation,glm::vec3 scale);


private:
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationMeshes;
    std::vector<glm::quat> rotationMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    void loadMesh(unsigned int indMesh);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTexture();

    std::vector<Vertex> assembleVertices
            (
                    std::vector<glm::vec3> positions,
                    std::vector<glm::vec3> normals,
                    std::vector<glm::vec2> texUVs
                    );

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};


#endif //GAME_MODEL_H
