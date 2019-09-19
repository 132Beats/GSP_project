//
// Created by hendrik on 30.04.19.
//

#include "geometry.hpp"
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

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

Geometry::Geometry() {
	camera_ = std::make_unique<Camera>();
}
Geometry::~Geometry() {

}

void Geometry::HandleKeyboardEvent() {

	if (keyboard_state_array[SDL_SCANCODE_A]) {
		alpha += 0.042;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_D]) {
		alpha -= 0.042;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_W]) {
        vx+=cos(alpha)*accconst;
        vz-=sin(alpha)*accconst;
		//beta += 0.02;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_S]) {
        vx-=cos(alpha)*accconst;
        vz+=sin(alpha)*accconst;
		//beta -= 0.02;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
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

void Geometry::Initialize(){
	ObjLoader objloader;
	
	objloader.loadOBJ("../../res/mod/tank.obj", verte, uvs, norm);
	
	glBufferData(GL_ARRAY_BUFFER, verte.size() * sizeof(glm::vec3), &verte[0], GL_STATIC_DRAW);
    alpha = 0;
    const int anzahl = 57;
    glm::vec3 v0 = glm::vec3(-0.6f, -0.6f, 0.39f);
    glm::vec3 v1 = glm::vec3(0.6f, -0.6f, 0.39f);
    glm::vec3 v2 = glm::vec3(0.0f, -0.6f, -0.78f);
    glm::vec3 v3 = glm::vec3(0.0f, 0.6f, 0.0f);
    float deg = 0.1f;
    glm::mat3x3 rotX = glm::mat3x3(glm::vec3(1, 0, 0), glm::vec3(0, cos(deg),sin(deg)),glm::vec3(0,-sin(deg),cos(deg)));


    //gen VAO genVertArr()
    glGenVertexArrays(1, &vertArrayObjNames);
    glBindVertexArray(vertArrayObjNames);

    //Gen VBO (genBuffer)
    glGenBuffers(1, &buffObjNames);
    glBindBuffer(GL_ARRAY_BUFFER, buffObjNames);
	glBufferData(GL_ARRAY_BUFFER, verte.size() * sizeof(glm::vec3), &verte[0], GL_STATIC_DRAW);

    //Gen VNO (f�r normalen)
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);

	//Gen UVBO
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);

	//Gen für Schatten
	frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	//Auflösung der shadowmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);
	//Nur Tiefentest keine Farbe
	glDrawBuffer(GL_NONE);


	initShaderProgram();



    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE,0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffObjNames);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, DIMENSION, GL_FLOAT, GL_FALSE,0, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(0.0f, 0.4275f, 0.8f, 1.0f);

	genMap();
}

void Geometry::genMap() {
	ObjLoader objloader;
	std::vector< glm::vec3 > verte2;
	std::vector< glm::vec2 > uvs2;
	std::vector< glm::vec3 > norm2;
	objloader.loadOBJ("../../res/mod/map.obj", verte2, uvs2, norm2);
	verteSize = verte2.size();

	//gen VAO genVertArr()
	glGenVertexArrays(1, &vaoMap);
	glBindVertexArray(vaoMap);

	//Gen VBO (vertexBuffer)
	glGenBuffers(1, &vertBuffMap);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffMap);
	glBufferData(GL_ARRAY_BUFFER, verte2.size() * sizeof(glm::vec3), &verte2[0], GL_STATIC_DRAW);

	//Gen VNO (f�r normalen)
	glGenBuffers(1, &normBuffMap);
	glBindBuffer(GL_ARRAY_BUFFER, normBuffMap);
	glBufferData(GL_ARRAY_BUFFER, norm2.size() * sizeof(glm::vec3), &norm2[0], GL_STATIC_DRAW);

	//Gen UVBO
	glGenBuffers(1, &uvBuffMap);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffMap);
	glBufferData(GL_ARRAY_BUFFER, norm2.size() * sizeof(glm::vec3), &norm2[0], GL_STATIC_DRAW);


}

glm::mat4x4 Geometry::getModelMat() {
	if (alpha >  2 * M_PI || alpha <  -2 * M_PI) alpha = -0 * M_PI;
	if (beta >  2 * M_PI || beta <  -2 * M_PI) beta = -0 * M_PI;
	rotY = glm::mat4x4(glm::vec4(cos(alpha), 0, -sin(alpha), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(alpha), 0, cos(alpha), 0),
		glm::vec4(0, 0, 0, 1));
	rotX = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cos(beta), sin(beta), 0),
		glm::vec4(0, -sin(beta), cos(beta), 0),
		glm::vec4(0, 0, 0, 1));
	tran = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, y, z, 1));
	return tran * rotY*rotX;
}
void Geometry::Physics() {
    x+=vx;
    y+=vy;
    z+=vz;
    vx= vx*29/30;
    vy= vy*29/30;
    vz= vz*29/30;
}

void Geometry::Render() {
	updateMatrix();
	genShadowMap();
	renderObjects();
}

void Geometry::updateMatrix(){
	m = getModelMat();
	v = camera_->getView();
	p = camera_->getPerspectiv();
}

void Geometry::genShadowMap(){
	glm::vec4 middle = m * glm::vec4(0.0, 0.0, 0.0, 1.0);
	lightInvDir = glm::vec3(-2.0f, 4.0f, -1.0f);
	float near_plane = 2.0f, far_plane = 10.0f;
	glm::mat4 sMProjection = glm::ortho(5.0f, -5.0f, -3.0f, +10.0f, -1.0f, +7.0f);
	glm::mat4 sMView = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 sMModel = m; 


	shadowMVP = sMProjection * sMView * sMModel;
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glUseProgram(shaderProgram));
	GLCall(glViewport(0, 0, 1024, 1024));
	//GLCall(glEnable(GL_CULL_FACE));
	//GLCall(glCullFace(GL_FRONT));

	GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "depthMVP"), 1, GL_FALSE, glm::value_ptr(shadowMVP)));

	GLCall(glBindVertexArray(vertArrayObjNames));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffObjNames));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));

	
	
}

void Geometry::renderObjects(){
	//Shader ändern
	GLCall(glUseProgram(program));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//Color und Depthbuffer leeren
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glViewport(0, 0, 800, 600));

	//Render Tank

	glm::mat4x4 mvp = p*v*m;
	glm::mat4 mv = glm::transpose(glm::inverse(m));
	glm::mat4 id = glm::mat4(1.0);
	//Richtiges vertexArray auswählen
	GLCall(glBindVertexArray(vertArrayObjNames));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffObjNames));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));

	//Uniforms setzen
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "shadowMVP"), 1, GL_FALSE, &shadowMVP[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "geometry_color"), 1, &glm::vec3(1.0, 1.0, 1.0)[0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MV"), 1, GL_FALSE, &mv[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "M"), 1, GL_FALSE, &m[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "light_position_worldspace"), 1, &glm::vec3(-2.0f, 4.0f, -1.0f)[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &camera_->getPosition()[0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 0);
	//Tank zeichnen
	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));
	

	//Render Map

	//Richtiges vertexArray auswählen
	GLCall(glBindVertexArray(vaoMap));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertBuffMap));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, normBuffMap));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
	//Modelmatrix auf "nicht bewegen" setzen (1er diagonale)
	m = id;
	//mvp neu berrechnen
	mvp = p * v*m;
	mv = glm::transpose(glm::inverse(m));
	//shadowMVP neu berrechnen
	shadowMVP = shadowVP * m;
	//Uniforms setzen
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MV"), 1, GL_FALSE, &mv[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "M"), 1, GL_FALSE, &m[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "shadowMVP"), 1, GL_FALSE, &shadowMVP[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "light_position_worldspace"), 1, &glm::vec3(-2.0f, 4.0f, -1.0f)[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &camera_->getPosition()[0]));
	//shadowmap setzen
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 0);
	//Map zeichnen
	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));
}

unsigned int Geometry::CompileShader(unsigned int type, const std::string& source){
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


void Geometry::initShaders(){
	const std::string vertexShaderCode = loadShader("../../res/shaders/vert.shader");
		
	const std::string fragmentShaderCode = loadShader("../../res/shaders/frag.shader");
		

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

void Geometry::initShaderProgram(){
	const std::string vertShader = loadShader("../../res/shaders/depthVert.shader");
	
	const std::string fragShader = loadShader("../../res/shaders/depthFrag.shader");

	shaderProgram = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vs);
	glDeleteShader(fs);

	
}

std::string Geometry::loadShader(const char* path) {
	std::ifstream shaderStream(path, std::ios::in);
	std::string code;
	if (shaderStream.is_open()) {
		std::stringstream st;
		st << shaderStream.rdbuf();
		return st.str();
	}
	printf("Fehler beim Laden");
	return 0;
}