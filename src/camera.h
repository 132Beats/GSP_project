//
// Created by hendrik on 28.05.19.
//

#ifndef GSP1_CAMERA_H
#define GSP1_CAMERA_H

#ifndef _WIN32
#include <glm/glm.hpp>
#else
#include <glm\glm.hpp>
#endif

class Camera {
	public:
		Camera();
		glm::mat4x4 getView();
		glm::mat4x4 getPerspectiv();
		glm::vec3 getPosition(){ return camPosition; };
	private:
		glm::vec3 camPosition, camTarget, userUp;
		glm::mat4x4 view, projection;
		float FoV,screenRatio, nCP, fCP;
		
};

#endif //GSP1_CAMERA_H
