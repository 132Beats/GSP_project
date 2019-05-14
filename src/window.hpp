#pragma once

#include <iostream>

#ifndef _WIN32 
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#else
#include <SDL.h>
#include <GL\glew.h>
#endif
class Window {
public:
	Window();
	~Window();
	void Show();
	void Clear();
	void Swap();
	void SetBackground(float red, float green, float blue, float alpha);
private:
	SDL_Window* sdl_window{ nullptr };
	SDL_GLContext context_{ nullptr };
};