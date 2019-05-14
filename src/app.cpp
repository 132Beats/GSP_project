
#include <iostream>
#include "app.hpp"
#include <memory>
#include <chrono>

#define DIMENSION 3
#define VERTEX_COUNT 4
#define INDEX_COUNT 12

#ifndef M_PI
#define M_PI	3.14159265358979323846f
#endif

static void GLClearError(){
    while(glGetError()!= GL_NO_ERROR){

    }
}

static bool GLPrintError(){
    while(GLenum error=glGetError()){
        std::cout << "[OpenGL Error] (" << error << std::endl;
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
int App::Start() {
	running_ = true;
	window_->Show();

    const std::string vertexShaderCode =
            "#version 330 core\n"
            "layout(location = 0) in vec3 position;\n"
            "void main(){\n"
            "gl_Position.xyz = position;\n"
            "gl_Position.w = 1.0;\n"
            "}\n";
    const std::string fragmentShaderCode =
            "#version 330 core\n"
            "layout(location = 0) out vec3 color;\n"
            "uniform vec3 user_color;\n"
            "void main(){\n"
            "color = vec3(0.443, 0.694, 0.153);\n"
            "}\n";
    GLuint vertArrayObjNames;
    GLuint buffObjNames;
    GLuint indexBuffer;
    float stdpositions[VERTEX_COUNT*DIMENSION] = {
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };
    float positions[VERTEX_COUNT*DIMENSION] = {
            -0.6f, -0.6f, 0.39f,
            0.6f, -0.5f, 0.39f,
            0.0f, -0.6f, -0.78f,
            0.0f, 0.6f, 0.0f
    };
    unsigned int indices[INDEX_COUNT] = {
            0,1,2,0,1,3,0,2,3,1,2,3
    };
    glGenVertexArrays(1,&vertArrayObjNames);
    glGenBuffers(1,&buffObjNames);
    glGenBuffers(1,&indexBuffer);

    glBindVertexArray(vertArrayObjNames);
    glBindBuffer(GL_ARRAY_BUFFER,buffObjNames);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBuffer);

    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*VERTEX_COUNT*DIMENSION,positions,GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*INDEX_COUNT,indices,GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, DIMENSION, GL_FLOAT,GL_FALSE, DIMENSION*sizeof(GL_FLOAT),0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    unsigned int shader = CreateShader(vertexShaderCode,fragmentShaderCode);
    glUseProgram(shader);

	while (running_) {
		HandleEvents();
		window_->Clear();
		Render();

        glBindVertexArray(vertArrayObjNames);
        glBindBuffer(GL_ARRAY_BUFFER,buffObjNames);

        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*VERTEX_COUNT*DIMENSION,positions,GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, DIMENSION, GL_FLOAT,GL_FALSE, DIMENSION*sizeof(GL_FLOAT),0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        float colore[3] = {1.0f,0.5f,0.0f};
        glUniform3fv(glGetUniformLocation(shader,"user_color"),3,colore);
        unsigned long int time = std::chrono::high_resolution_clock().now().time_since_epoch().count();
        double timed = time;
        timed=timed/500000000;
        std::cout << time << " : " << positions[0] << " : " << (timed)<< std::endl;

        /*positions[0]=stdpositions[0]*sin(timed);
        positions[3]=stdpositions[3]*sin(timed);
        positions[6]=stdpositions[6]*sin(timed);


        positions[1]=stdpositions[1]*sin(timed+M_PI/2);
        positions[4]=stdpositions[4]*sin(timed+M_PI/2);
        positions[7]=stdpositions[7]*sin(timed+M_PI/2);*/
        glDrawArrays(GL_TRIANGLES, 0, 4);
        GLClearError();
        glDrawElements(GL_TRIANGLES,INDEX_COUNT,GL_UNSIGNED_INT,nullptr);
        std::cout << "INDICES:" << std::endl;
        GLPrintError();

		window_->Swap();
	}
	glDeleteProgram(shader);

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
			HandleKeyboardEvent(event.key);
		}
	}
}
void App::HandleKeyboardEvent(const SDL_KeyboardEvent& event) {
	switch (event.type) {
	case SDL_KEYUP:
		switch (event.keysym.sym) {
		case SDLK_r:
			window_->SetBackground(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		case SDLK_b:
			window_->SetBackground(0.0f, 0.0f, 1.0f, 0.0f);
			break;
		}
	}
}
void App::Render() {
	
}

unsigned int App::CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if (result==GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        std::cout
        <<"Failed to compile "
        << (type==GL_VERTEX_SHADER ? "vertex" : "fragment")
        << "shader!"
        << std::endl
        << message
        << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int App::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program,vs);
    glAttachShader(program,fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}