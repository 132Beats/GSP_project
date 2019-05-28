#pragma once

#include "window.hpp"
#include <memory>

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
	GLuint vertArrayObjNames, buffObjNames, indexBuffer, program;
	bool running_{ false };
	void HandleEvents();
	void HandleKeyboardEvent(const SDL_KeyboardEvent& event);
	void Render();
	void initShaders();
	void initGeo();
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	std::unique_ptr<Window> window_;
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