
#include <iostream>
#include "app.hpp"
#include <memory>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#define DIMENSION 3
#define GLCall(x) GLClearError();x;GLPrintError(#x, __FILE__, __LINE__);
#ifndef M_PI
#define M_PI	3.14159265358979323846f
#endif

static void GLClearError(){
    while(glGetError()!= GL_NO_ERROR){

    }
}

static bool GLPrintError(const char* function, const char* file, int line){
    while(GLenum error=glGetError()){
        std::cout << "[OpenGL Error:" << error <<"] @ " << function << " on line " << line << " in " << file << std::endl;
        return false;
    }
    return true;
}

App::App() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initiate SDL: %s", SDL_GetError());
		std::cerr << "Could not initialize SDL2" << std::endl;
		throw("Could not initialize SDL2");
	}
	else{
		window_ = std::make_unique<Window>();
	}
}
App::~App() {
	SDL_Quit();
}

std::string App::GetShader(const std::string &path){
    std::ifstream stream(path);
    std::string line,str;
    std::stringstream ss;
    while(getline(stream,line)){
        ss << line << std::endl;
        str = str + line + "\n";
    }
    std::cout << "String: " << str << std::endl;
    return str;
}
int App::Start() {
	running_ = true;
	window_->Show();

	initGeo();
	initShaders();
	alpha = 0;
	while (running_) {
		HandleEvents();
		window_->Clear();
		Render();
		window_->Swap();
		//_sleep(5);
	}
	glDeleteProgram(program);
	return 0;
}
void App::HandleEvents() {
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running_ = false;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			HandleKeyboardEvent();
		}
	}
}
void App::HandleKeyboardEvent() {
	
	if(keyboard_state_array[SDL_SCANCODE_A]) {
		alpha += 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_D]) {
		alpha -= 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_W]) {
		beta += 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_S]) {
		beta -= 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_UP]) {
		y += 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_DOWN]) {
		y -= 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_RIGHT]) {
		x += 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_LEFT]) {
		x -= 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_B]) {
		z += 0.02;
	}
	if (keyboard_state_array[SDL_SCANCODE_V]) {
		z -= 0.02;
	}
	/*if (keyboard_state_array[SDL_SCANCODE_]) {
	}
		
	}*/
}
void App::Render() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	if (alpha >  2 * M_PI || alpha <  -2 * M_PI) alpha = -0 * M_PI;
	if (beta >  2 * M_PI || beta <  -2 * M_PI) beta = -0 * M_PI;
	rotY = glm::mat4x4(glm::vec4(cos(alpha), 0, -sin(alpha), 0),
		glm::vec4(0, 1,0 , 0),
		glm::vec4(sin(alpha),0 , cos(alpha), 0),
		glm::vec4(0, 0, 0, 1));
	rotX = glm::mat4x4(glm::vec4(1,0,0,0),
		glm::vec4(0,cos(beta),sin(beta),0),
		glm::vec4(0,-sin(beta),cos(beta),0),
		glm::vec4(0,0,0,1));
	tran = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, y, z, 1));
	rot = tran*rotX*rotY;
	//printf("%f %f %f %f\n", rotY[0].y, rotY[1].y, rotY[2].y,rotY[3].y);
    GLCall(glBindVertexArray(vertArrayObjNames));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffObjNames));
	//glm::mat4 rot = glm::ro
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
    GLCall(glUniform3f(glGetUniformLocation(program, "geometry_color"), 1.0f, 1.0f, 1.0f));
    GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "rot"), 1, GL_FALSE, &rot[0][0]));
    GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "rotT"), 1, GL_FALSE, &glm::transpose(rot)[0][0]));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
    GLCall(glUniform3f(glGetUniformLocation(program, "geometry_color"), 1.0f, 0.0f, 0.0f));
    GLCall(glDrawArrays(GL_TRIANGLES, 3, 3));
    GLCall(glUniform3f(glGetUniformLocation(program, "geometry_color"), 0.0f, 1.0f, 0.0f));
    GLCall(glDrawArrays(GL_TRIANGLES, 6, 3));
	GLCall(glUniform3f(glGetUniformLocation(program, "geometry_color"), 0.0f, 0.0f, 1.0f));
    GLCall(glDrawArrays(GL_TRIANGLES, 9, 3));
}

unsigned int App::CompileShader(unsigned int type, const std::string& source){
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id,1,&src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id,GL_COMPILE_STATUS,&result));
    if (result==GL_FALSE){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length*sizeof(char));
        GLCall(glGetShaderInfoLog(id,length,&length,message));
        std::cout
        <<"Failed to compile "
        << (type==GL_VERTEX_SHADER ? "vertex" : "fragment")
        << "shader!"
        << std::endl
        << message
        << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}


void App::initShaders(){
	const std::string vertexShaderCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"uniform mat4 rot;\n"
		"uniform mat4 rotT;\n"
		"out vec3 vertex_normal_worldspace\n;"
		"void main(){\n"
		"gl_Position = rot * vec4(position,1);\n"
		//"gl_Position.w = 1.0;\n"
		"vertex_normal_worldspace = (rotT*vec4(normal,1)).xyz;"
		"}\n";
	const std::string fragmentShaderCode =
		"#version 330 core\n"
		"layout(location = 0) out vec3 color;\n"
		"uniform vec3 geometry_color;\n"
		"in vec3 vertex_normal_worldspace;"
		"void main(){\n"
		"float nz = 0.5+0.5*vertex_normal_worldspace.z;\n"
		"float factor = abs(nz);\n"
		"color = factor*geometry_color;\n"
		"}\n";

	program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(program); 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void App::initGeo(){
	const int anzahl = 36;
	glm::vec3 v0 = glm::vec3(-0.6f, -0.6f, 0.39f);
	glm::vec3 v1 = glm::vec3(0.6f, -0.6f, 0.39f);
	glm::vec3 v2 = glm::vec3(0.0f, -0.6f, -0.78f);
	glm::vec3 v3 = glm::vec3(0.0f, 0.6f, 0.0f);
	float deg = 0.1f;
	glm::mat3x3 rotX = glm::mat3x3(glm::vec3(1, 0, 0), glm::vec3(0, cos(deg),sin(deg)),glm::vec3(0,-sin(deg),cos(deg)));
	

	/*v0 = rotX*v0;
	v1 = rotX*v1;
	v2 = rotX*v2;
	v3 = rotX*v3;

	/*glm::mat3x3 rotX = glm::mat4x4(glm::vec4(cos(alpha), 0, sin(alpha), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(-sin(alpha), 0, cos(alpha), 0),
		glm::vec4(0, 0, 0, 1));
	
	glm::vec4 tmp = glm::vec4(v0, 1);
	tmp = rotX*tmp;
	v0.x = tmp.x;
	v0.y = tmp.y;
	v0.z = tmp.z;

	tmp = glm::vec4(v1, 1);
	tmp = rotZ*tmp;
	v1.x = tmp.x;
	v1.y = tmp.y;
	v1.z = tmp.z;

	tmp = glm::vec4(v2, 1);
	tmp = rotZ*tmp;
	v2.x = tmp.x;
	v2.y = tmp.y;
	v2.z = tmp.z;

	tmp = glm::vec4(v3, 1);
	tmp = rotZ*tmp;
	v3.x = tmp.x;
	v3.y = tmp.y;
	v3.z = tmp.z;
	*/
	

	float positions[anzahl] = {
		v0.x, v0.y, v0.z,
		v2.x, v2.y, v2.z,
		v3.x, v3.y, v3.z,

		v2.x, v2.y, v2.z,
		v1.x, v1.y, v1.z,
		v3.x, v3.y, v3.z,

		v0.x, v0.y, v0.z,
		v3.x, v3.y, v3.z,
		v1.x, v1.y, v1.z,

		v0.x, v0.y, v0.z,
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z
	};
	
	glm::vec3 n0 = glm::cross(v2 - v0, v3 - v0);
	n0 = n0 / glm::abs(n0);
	glm::vec3 n1 = glm::cross(v1 - v2, v3 - v2);
	n1 = n1 / glm::abs(n1);
	glm::vec3 n2 = glm::cross(v0 - v1, v3 - v1);
	n2 = n2 / glm::abs(n2);
	glm::vec3 n3 = glm::cross(v1 - v0, v2 - v0);
	n3 = n3 / glm::abs(n3);

	float normals[anzahl] = {
		n0.x, n0.y, n0.z,
		n0.x, n0.y, n0.z,
		n0.x, n0.y, n0.z,

		n1.x, n1.y, n1.z,
		n1.x, n1.y, n1.z,
		n1.x, n1.y, n1.z,

		n2.x, n2.y, n2.z,
		n2.x, n2.y, n2.z,
		n2.x, n2.y, n2.z,

		n3.x, n3.y, n3.z,
		n3.x, n3.y, n3.z,
		n3.x, n3.y, n3.z
	};
	printf("N0: %f %f %f\n", n0.x, n0.y, n0.z);
	printf("V1: %f %f %f\n", v1.x, v1.y, v1.z);
	printf("V2: %f %f %f\n", v2.x, v2.y, v2.z);
	printf("V3: %f %f %f\n", v3.x, v3.y, v3.z);
	//gen VAO genVertArr()
	glGenVertexArrays(1, &vertArrayObjNames);
	glBindVertexArray(vertArrayObjNames);

	//Gen VBO (genBuffer)
	glGenBuffers(1, &buffObjNames);
	glBindBuffer(GL_ARRAY_BUFFER, buffObjNames);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*anzahl, positions, GL_STATIC_DRAW);
	
	//Gen VNO (f�r normalen)
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*anzahl, normals, GL_STATIC_DRAW);


	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE,0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, buffObjNames);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, DIMENSION, GL_FLOAT, GL_FALSE,0, 0);
}