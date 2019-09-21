#pragma once

#include <GL/glew.h>
#include <iostream>
#include <memory>
#include "camera.h"
#include <fstream>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "objLoader.hpp"
#ifndef _WIN32 
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#else
#include <SDL.h>
#include <glm\glm.hpp>
#endif

class Geometry {
	std::vector< glm::vec3 > verte;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > norm;
public:
    Geometry();
    ~Geometry();
	void genMap();
    void Render();
    void Physics();
	void updateMatrix();
	void genShadowMap();
	void renderObjects(); 
    void Initialize();
	void initShaderProgram();
	void HandleKeyboardEvent();
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void initShaders();
private:
    GLuint vertArrayObjNames, buffObjNames, normalBuffer, uvBuffer, program, shadowMap, frameBuffer, shaderProgram,
			vaoMap, vertBuffMap, normBuffMap, uvBuffMap;
	int verteSizeMap;
	glm::mat4x4 rotYTankA, rotYTankB, rotXTankA, rotXTankB;
	glm::mat4x4 mTankA, mTankB, v, p, shadowMVPMap, shadowMVPTankA, shadowMVPTankB;
	glm::mat4x4 tranTankA, tranTankB;
	glm::vec3 lightInvDir;
	float accconst = 0.008;
	float alphaTankA = 0, alphaTankB = 0;
	float betaTankA = 0, betaTankB = 0;
	float xTankA = -3, xTankB = 3;
	float yTankA = 1, yTankB = 1;
	float zTankA = -3, zTankB = 3;
	float vxTankA = 0, vxTankB = 0; //Geschwindigkeit in x-Richtung
	float vyTankA = 0, vyTankB = 0; //Geschwindigkeit in y-Richtung
	float vzTankA = 0, vzTankB = 0; //Geschwindigkeit in z-Richtung
	std::unique_ptr<Camera> camera_;
	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	glm::mat4x4 getModelMatTankA();
	glm::mat4x4 getModelMatTankB();
	std::string loadShader(const char* path);
	
};