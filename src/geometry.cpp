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
		alphaTankA += 0.042;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_D]) {
		alphaTankA -= 0.042;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_W]) {
        vxTankA+=cos(alphaTankA)*accconst;
        vzTankA-=sin(alphaTankA)*accconst;
		//beta += 0.02;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_S]) {
        vxTankA-=cos(alphaTankA)*accconst;
        vzTankA+=sin(alphaTankA)*accconst;
		//beta -= 0.02;
        //std::cout << "alpha:" << alpha <<std::endl << "beta:"  << beta << std::endl;
	}
	if (keyboard_state_array[SDL_SCANCODE_UP]) {
		vxTankB += cos(alphaTankB)*accconst;
		vzTankB -= sin(alphaTankB)*accconst;
	}
	if (keyboard_state_array[SDL_SCANCODE_DOWN]) {
		vxTankB -= cos(alphaTankB)*accconst;
		vzTankB += sin(alphaTankB)*accconst;
	}
	if (keyboard_state_array[SDL_SCANCODE_RIGHT]) {
		alphaTankB -= 0.042;
	}
	if (keyboard_state_array[SDL_SCANCODE_LEFT]) {
		alphaTankB += 0.042;
	}
	/*if (keyboard_state_array[SDL_SCANCODE_]) {
	}

	}*/
}

void Geometry::Initialize(){
	ObjLoader objloader;
	
	objloader.loadOBJ("../../res/mod/tank.obj", verte, uvs, norm);
	
	//glBufferData(GL_ARRAY_BUFFER, verte.size() * sizeof(glm::vec3), &verte[0], GL_STATIC_DRAW);
    
	lightInvDir = glm::vec3(-2.0f, 4.0f, -1.0f);

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
	glClearColor(0.13f, 0.44f, 0.7f, 1.0f);

	loadMap();
}

void Geometry::loadMap() {
	ObjLoader objloader;
	std::vector< glm::vec3 > verte2;
	std::vector< glm::vec2 > uvs2;
	std::vector< glm::vec3 > norm2;
	objloader.loadOBJ("../../res/mod/map_noise.obj", verte2, uvs2, norm2);
	verteSizeMap = verte2.size();

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

/*
*Die Kugel liegt im speicher und kann nun gerendert werden.
*/
void Geometry::loadBullet(){
	ObjLoader objloader;
	std::vector< glm::vec3 > verte2;
	std::vector< glm::vec2 > uvs2;
	std::vector< glm::vec3 > norm2;
	objloader.loadOBJ("../../res/mod/bullet.obj", verte2, uvs2, norm2);
	verteSizeBullet = verte2.size();

	//gen VAO genVertArr()
	glGenVertexArrays(1, &vaoBullet);
	glBindVertexArray(vaoBullet);

	//Gen VBO (vertexBuffer)
	glGenBuffers(1, &vertBullet);
	glBindBuffer(GL_ARRAY_BUFFER, vertBullet);
	glBufferData(GL_ARRAY_BUFFER, verte2.size() * sizeof(glm::vec3), &verte2[0], GL_STATIC_DRAW);

	//Gen VNO (f�r normalen)
	glGenBuffers(1, &normBullet);
	glBindBuffer(GL_ARRAY_BUFFER, normBuffMap);
	glBufferData(GL_ARRAY_BUFFER, norm2.size() * sizeof(glm::vec3), &norm2[0], GL_STATIC_DRAW);

}

glm::mat4x4 Geometry::getModelMatTankA() {
	if (alphaTankA >  2 * M_PI || alphaTankA <  -2 * M_PI) alphaTankA = -0 * M_PI;
	if (betaTankA >  2 * M_PI || betaTankA <  -2 * M_PI) betaTankA = -0 * M_PI;
	rotYTankA = glm::mat4x4(glm::vec4(cos(alphaTankA), 0, -sin(alphaTankA), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(alphaTankA), 0, cos(alphaTankA), 0),
		glm::vec4(0, 0, 0, 1));
	rotXTankA = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cos(betaTankA), sin(betaTankA), 0),
		glm::vec4(0, -sin(betaTankA), cos(betaTankA), 0),
		glm::vec4(0, 0, 0, 1));
	tranTankA = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(xTankA, yTankA, zTankA, 1));
	return tranTankA * rotYTankA*rotXTankA;
}


glm::mat4x4 Geometry::getModelMatTankB() {
	if (alphaTankB >  2 * M_PI || alphaTankB <  -2 * M_PI) alphaTankB = -0 * M_PI;
	if (betaTankB >  2 * M_PI || betaTankB <  -2 * M_PI) betaTankB = -0 * M_PI;
	rotYTankB = glm::mat4x4(glm::vec4(cos(alphaTankB), 0, -sin(alphaTankB), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(alphaTankB), 0, cos(alphaTankB), 0),
		glm::vec4(0, 0, 0, 1));
	rotXTankB = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cos(betaTankB), sin(betaTankB), 0),
		glm::vec4(0, -sin(betaTankB), cos(betaTankB), 0),
		glm::vec4(0, 0, 0, 1));
	tranTankB = glm::mat4x4(glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(xTankB, yTankB, zTankB, 1));
	return tranTankB * rotYTankB *rotXTankB;
}

void Geometry::Physics() {
    xTankA+=vxTankA;
    yTankA+=vyTankA;
    zTankA+=vzTankA;
    double vgesamtTankA=sqrt((vxTankA*vxTankA)+(vyTankA*vyTankA)+(vzTankA*vzTankA));
    float abbremsungA = vgesamtTankA*vgesamtTankA*luftreibungskonstante;
    float abbremsungsfaktorA=1;
    if(vgesamtTankA!=0) {
        abbremsungsfaktorA = (vgesamtTankA-abbremsungA-rollreibungskonstante) / vgesamtTankA;
    }
    std::cout << abbremsungsfaktorA << std::endl;

    vxTankA= vxTankA*abbremsungsfaktorA;
    vyTankA= vyTankA*abbremsungsfaktorA;
    vzTankA= vzTankA*abbremsungsfaktorA;



	xTankB += vxTankB;
	yTankB += vyTankB;
	zTankB += vzTankB;
    double vgesamtTankB=sqrt((vxTankB*vxTankB)+(vyTankB*vyTankB)+(vzTankB*vzTankB));
    float abbremsungB = vgesamtTankB*vgesamtTankB*luftreibungskonstante;
    float abbremsungsfaktorB=1;
    if(vgesamtTankB!=0) {
        abbremsungsfaktorB = (vgesamtTankB-abbremsungB-rollreibungskonstante) / vgesamtTankB;
    }
    std::cout << abbremsungsfaktorB << std::endl;

    vxTankB= vxTankB*abbremsungsfaktorB;
    vyTankB= vyTankB*abbremsungsfaktorB;
    vzTankB= vzTankB*abbremsungsfaktorB;
}

void Geometry::Render() {
	updateMatrix();
	genShadowMap();
	renderObjects();
}

void Geometry::updateMatrix(){
	mTankA = getModelMatTankA();
	mTankB = getModelMatTankB();
	v = camera_->getView();
	p = camera_->getPerspectiv();
}

void Geometry::genShadowMap(){
	
	
	glm::mat4 sMProjection = glm::ortho(25.0f, -25.0f, -25.0f, +25.0f, -25.0f, 25.0f);
	glm::mat4 sMView = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glUseProgram(shaderProgram));
	GLCall(glViewport(0, 0, 1024, 1024));
	//GLCall(glEnable(GL_CULL_FACE));
	//GLCall(glCullFace(GL_FRONT));

	
	//Render TankA
	GLCall(glBindVertexArray(vertArrayObjNames));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffObjNames));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));

	shadowMVPTankA = sMProjection * sMView * mTankA;

	GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "depthMVP"), 1, GL_FALSE, glm::value_ptr(shadowMVPTankA)));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));

	//Render TankB
	shadowMVPTankB = sMProjection * sMView * mTankB;

	GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "depthMVP"), 1, GL_FALSE, glm::value_ptr(shadowMVPTankB)));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));

	shadowMVPMap = sMProjection * sMView * glm::mat4(1.0);
	/*
	//Render Map
	GLCall(glBindVertexArray(vaoMap));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertBuffMap));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));


	GLCall(glDrawArrays(GL_TRIANGLES, 0, verteSizeMap));*/
	
	
}

void Geometry::renderObjects(){
	//Shader ändern
	GLCall(glUseProgram(program));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//Color und Depthbuffer leeren
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glViewport(0, 0, 1280, 720));

	//Render TankA

	glm::mat4x4 mvp = p*v*mTankA;
	glm::mat4 mv = glm::transpose(glm::inverse(mTankA));
	glm::mat4 id = glm::mat4(1.0);

	//Richtiges vertexArray auswählen
	GLCall(glBindVertexArray(vertArrayObjNames));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffObjNames));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, DIMENSION, GL_FLOAT, GL_FALSE, 0, 0));

	//Uniforms setzen
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "shadowMVP"), 1, GL_FALSE, &shadowMVPTankA[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "geometry_color"), 1, &glm::vec3(0.13, 0.54, 0.13)[0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MV"), 1, GL_FALSE, &mv[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "M"), 1, GL_FALSE, &mTankA[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "light_position_worldspace"), 1, &glm::vec3(-2.0f, 4.0f, -1.0f)[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &camera_->getPosition()[0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 0);

	//Tank zeichnen
	GLCall(glDrawArrays(GL_TRIANGLES, 0, verte.size()));
	
	//Render TankB

	mvp = p*v*mTankB;
	mv = glm::transpose(glm::inverse(mTankB));

	//Uniforms setzen
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "shadowMVP"), 1, GL_FALSE, &shadowMVPTankB[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "geometry_color"), 1, &glm::vec3(1.0, 0.0, 0.0)[0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MV"), 1, GL_FALSE, &mv[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "M"), 1, GL_FALSE, &mTankB[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "light_position_worldspace"), 1, &glm::vec3(-2.0f, 4.0f, -1.0f)[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &camera_->getPosition()[0]));
	
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
	
	//mvp neu berrechnen
	mvp = p * v*id;
	mv = glm::transpose(glm::inverse(id));

	//shadowMVP neu berrechnen
	//Uniforms setzen
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "MV"), 1, GL_FALSE, &mv[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "M"), 1, GL_FALSE, &id[0][0]));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "shadowMVP"), 1, GL_FALSE, &shadowMVPMap[0][0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "light_position_worldspace"), 1, &glm::vec3(-2.0f, 4.0f, -1.0f)[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &camera_->getPosition()[0]));
	GLCall(glUniform3fv(glGetUniformLocation(program, "geometry_color"), 1, &glm::vec3(0.95, 0.7, 0.4)[0]));
	//shadowmap setzen
	
	//Map zeichnen
	GLCall(glDrawArrays(GL_TRIANGLES, 0, verteSizeMap));
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