#include "misc.hpp"
#include "camera.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/rotate_vector.hpp>

#include <iostream>

#define LOOK_VEC_THRESHHOLD 0.9

std::ostream& operator<<(std::ostream& os, const glm::vec3 vec)
{
	os << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2 vec)
{
	os << "{" << vec.x << ", " << vec.y << "}";
	return os;
}

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

	glm::vec3 lookVec = glm::normalize(_lookingAt - _location);

	glm::vec3 perpendicular = glm::cross(lookVec, _upDirection);
	glm::mat3 parallelRotation = glm::mat3(glm::rotate(-step*deltaX, _upDirection));
	
	// Looking direction and up direction cannot be the same
	if (glm::dot(lookVec, _upDirection) > LOOK_VEC_THRESHHOLD)
		deltaY = deltaY > 0 ? deltaY : 0;
	else if (glm::dot(lookVec, _upDirection) < -LOOK_VEC_THRESHHOLD)
		deltaY = deltaY < 0 ? deltaY : 0;

	glm::mat3 perpendicularRotation = glm::mat3(glm::rotate(-step*deltaY, perpendicular));

	_lookingAt = parallelRotation*perpendicularRotation*lookVec + _location;

	x = newX;
	y = newY;
}

bool Camera::lookVecTooCloseToUpDirection(glm::vec3 lookVec)
{
	glm::vec3 nLookVec = glm::normalize(lookVec);
	float dotProduct = glm::dot(nLookVec, _upDirection);
	return dotProduct > 0.9 || dotProduct < -0.9;
}

void Camera::takeStep(glm::vec3 directionRelativeToCamera)
{
	static const float stepLength = 0.2f;

	glm::vec3 forward = _location - _lookingAt;
	forward = forward - glm::dot(forward, _upDirection)*_upDirection; // Limit movement to a plane

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

glm::mat4 DragCamera::WTVMatrix()
{
	return glm::lookAt(_location, _lookingAt, _upDirection);
}


void DragCamera::anchor(glm::vec2 point)
{
	_anchor = point;
	_velocity = {0, 0};
}

void DragCamera::updatePosition(glm::vec2 toPoint)
{
	glm::vec2 direction = toPoint - _anchor;
	calculateNewVectorsForDirection(direction);
	_anchor = toPoint;
}

void DragCamera::updatePosition()
{
	_velocity /= 1.1;
	calculateNewVectorsForDirection(_velocity);
}

void DragCamera::calculateNewVectorsForDirection(glm::vec2 direction)
{
	_velocity = direction;
	if (glm::length(direction) == 0)
		return;

	glm::vec3 forward = glm::normalize(_location - _lookingAt);
	glm::vec3 perpendicular = glm::cross(forward, _upDirection);

	glm::vec3 movementVec = glm::normalize(direction.x*perpendicular + direction.y*_upDirection);
	glm::vec3 rotationAxis = glm::cross(movementVec, forward);

	static const float angle = 0.01;
	glm::mat3 rotationMatrix = glm::mat3(glm::rotate(-glm::length(direction)*angle, rotationAxis));

	_location = rotationMatrix*_location;
	_lookingAt = rotationMatrix*_lookingAt;
	_upDirection = rotationMatrix*_upDirection;
}
