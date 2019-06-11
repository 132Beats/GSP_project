#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
class Geometry {
public:
    Geometry();
    ~Geometry();
    void Render();
    void Initialize();
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void initShaders();
private:
    GLuint vertArrayObjNames, buffObjNames, normalBuffer, program;
    glm::mat4x4 rotY;
    glm::mat4x4 rotX;
    glm::mat4x4 rot;
    glm::mat4x4 tran;
    float alpha = 0;
    float beta = 0;
    float x = 0;
    float y = 0;
    float z = 0;
};