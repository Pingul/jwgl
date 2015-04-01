#include "misc.hpp"
#include "camera.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

int sign(int value)
{
	if (value < 0)
		return -1;
	if (value > 0)
		return 1;
	return 0;
}

void Camera::reorient(int newX, int newY)
{
	static int x;
	static int y;
	static const float step = M_PI/72;

	int deltaX = sign(newX - x);
	int deltaY = sign(newY - y);

	glm::vec3 lookVec = _lookingAt - _location;

	glm::vec3 perpendicular = glm::cross(lookVec, _upDirection);
	glm::mat3 parallelRotation = glm::mat3(glm::rotate(-step*deltaX, _upDirection));
	glm::mat3 perpendicularRotation = glm::mat3(glm::rotate(-step*deltaY, perpendicular));

	_lookingAt = parallelRotation*perpendicularRotation*lookVec + _location;

	x = newX;
	y = newY;
}

void Camera::takeStep(glm::vec3 directionRelativeToCamera)
{
	static const float stepLength = 0.2f;

	glm::vec3 forward = _location - _lookingAt;
	forward = forward - glm::dot(forward, _upDirection); // Limit movement to a plane
	glm::vec3 perpendicular = glm::cross(forward, _upDirection);

	glm::vec3 sideTranslation = -stepLength*directionRelativeToCamera.x*glm::normalize(perpendicular);
	glm::vec3 heightTranslation = stepLength*directionRelativeToCamera.y*_upDirection;
	glm::vec3 depthTranslation = -stepLength*directionRelativeToCamera.z*glm::normalize(forward);
	glm::vec3 translation = sideTranslation + depthTranslation + heightTranslation;

	_location = translation + _location;
	_lookingAt = translation + _lookingAt;
}


glm::mat4 Camera::WTVMatrix()
{
	return glm::lookAt(_location, _lookingAt, _upDirection);
}