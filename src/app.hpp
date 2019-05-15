#pragma once

#include "window.hpp"
#include <memory>

#ifndef _WIN32 
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

class App {
public:
	App();
	~App();
	int Start();
private:
	bool running_{ false };
	void HandleEvents();
	void HandleKeyboardEvent(const SDL_KeyboardEvent& event);
	void Render();
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	std::unique_ptr<Window> window_;
    std::string GetShader(const std::string &path);
};