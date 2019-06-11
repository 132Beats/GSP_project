#pragma once

#include "window.hpp"
#include "geometry.hpp"
#include <memory>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <iostream>

#ifndef _WIN32 
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#else
#include <SDL.h>
#include <glm\glm.hpp>
#endif

class App {
public:
	App();
	~App();
	int Start();
private:
	GLuint vertArrayObjNames, buffObjNames, normalBuffer, program;
	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	bool running_{ false };
	void HandleEvents();
	void HandleKeyboardEvent();
	void Render();
	void initShaders();
	void initGeo();
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	std::unique_ptr<Window> window_;
    std::unique_ptr<Geometry> geometry_;
    std::string GetShader(const std::string &path);
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