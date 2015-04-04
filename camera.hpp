#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>

class Camera
{
	public:
		Camera(glm::vec3 location, glm::vec3 lookingAt, glm::vec3 upDirection) :
			_location(location), _lookingAt(lookingAt), _upDirection(upDirection) {}
		~Camera() = default;

		void reorient(int newX, int newY);
		void takeStep(glm::vec3 directionRelativeToCamera);
		glm::mat4 WTVMatrix();

	private:
		glm::vec3 _location;
		glm::vec3 _lookingAt;
		glm::vec3 _upDirection;

		bool lookVecTooCloseToUpDirection(glm::vec3 lookVec);
};

#endif