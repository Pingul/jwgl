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
		glm::vec3 at() { return _location; }

	private:
		glm::vec3 _location;
		glm::vec3 _lookingAt;
		glm::vec3 _upDirection;

		bool lookVecTooCloseToUpDirection(glm::vec3 lookVec);
};

// class DragCamera // These should maybe have a common main class
// {
// 	public:
// 		DragCamera() = default;
// 		~DragCamera() = default;

// 		void updateDirection(int newX, int newY);
// 		void 

// 	private: 
// 		glm::vec3 _location{0, 0, 5};
// 		glm::vec3 _lookingAt{0, 0, 0};
// 		glm::vec3 _upDirection{0, 1, 0};

// 		glm::vec3 _movement{0, 0, 0}; // Using this to have some feeling of momentum

// };


#endif