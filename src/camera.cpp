//
// Created by hendrik on 28.05.19.
//

#include "camera.h"


Camera::Camera() {
	e = glm::vec3(0, 0, 0);
	userUp = glm::vec3(0, 1, 0);
	c = glm::vec3(0, 0, -1);
	paramChanged();
}

void Camera::paramChanged() {
	g = c - e;
	back = -(g / glm::abs(g));
	s = glm::cross(g, userUp);
	s = s / glm::abs(s);
	u = glm::cross(back, s);
}

glm::mat4x4 Camera::getRot() {
	return glm::mat4x4(glm::vec4(s,0), glm::vec4(u,0), glm::vec4(back,0),glm::vec4(0,0,0,1));
}

glm::mat4x4 Camera::getTrans() {
	return glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(e, 1));
}

glm::mat4x4 Camera::getWorldToCam() {
	return glm::transpose(getRot()) * glm::transpose(getTrans());
}

