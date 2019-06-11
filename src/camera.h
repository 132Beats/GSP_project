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
		glm::mat4x4 getWorldToCam();
		glm::mat4x4 getRot();
		glm::mat4x4 getTrans();
	private:
		glm::vec3 e, c, userUp, g, back, s, u;
		int r, l, b, t, n, rF, lF, bF, tF, f; // near n und far f sind -f und -n


		void paramChanged();
		
};

#endif //GSP1_CAMERA_H
