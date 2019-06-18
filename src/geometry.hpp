#pragma once

#include <GL/glew.h>
#include <iostream>
#include <memory>
#include "camera.h"
#ifndef _WIN32 
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#else
#include <SDL.h>
#include <glm\glm.hpp>
#endif

class Geometry {
public:
    Geometry();
    ~Geometry();
    void Render();
    void Initialize();
	void HandleKeyboardEvent();
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void initShaders();
private:
    GLuint vertArrayObjNames, buffObjNames, normalBuffer, program;
    glm::mat4x4 rotY;
    glm::mat4x4 rotX;
    glm::mat4x4 m,v,p;
    glm::mat4x4 tran;
    float alpha = 0;
    float beta = 0;
    float x = 0;
    float y = 0;
    float z = 0;
	std::unique_ptr<Camera> camera_;
	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	glm::mat4x4 getModelMat();
	
};