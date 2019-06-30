//
// Created by hendrik on 28.05.19.
//

#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"


Camera::Camera() {
	camPosition = glm::vec3(0,0, 5);
	camTarget = glm::vec3(0, 0, 0);
	userUp = glm::vec3(0, 1, 0);
	view = glm::lookAt(camPosition, camTarget, userUp);
	FoV = 45.0f;
	nCP = 0.1f;
	fCP = 100.0f;
	
	screenRatio = 800.0f / 600.0f;
	projection = glm::perspective(glm::radians(FoV), screenRatio, nCP, fCP);
	//projection = glm::ortho(-25.0f, 25.0f, -1.0f, 25.0f, 0.001f, 100.0f);
}

glm::mat4 Camera::getPerspectiv() {
	return projection;
}

glm::mat4 Camera::getView() {
	return view;
}