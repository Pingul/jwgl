#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>

class Camera
{
	public:
		Camera() = default;
		~Camera() = default;

		void reorient(int newX, int newY);
		void takeStep(glm::vec3 directionRelativeToCamera);
		glm::mat4 WTVMatrix();

		static Camera* defaultCamera();

	private:
		glm::vec3 _location;
		glm::vec3 _lookingAt;
		glm::vec3 _upDirection;
};

#endif