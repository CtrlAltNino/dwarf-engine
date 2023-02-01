#pragma once

#include "../../utilities/dpch.h"

//#include<glad/glad.h>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//#include"../data structures/Quaternion.h"

/// @brief This class represents a transform. It manages and contains a position, a scale and a rotation.
class Transform {
private:
	/// @brief The position of the transform.
	glm::vec3 position;

	/// @brief The scale of the transform.
	glm::vec3 scale;

	/// @brief The rotation of the transform.
	glm::quat rotation;
public:
	// ========== Constructors ==========
	
	Transform(glm::vec3 position);
	Transform(glm::vec3 position, glm::quat rotation);
	Transform(glm::vec3 position, glm::vec3 eulerAngles);

	// ========== Getters ==========
	
	/// @brief Returns the position of the transform.
	/// @return A 3D position vector.
	glm::vec3 getPosition();

	/// @brief Returns the rotation of the transform.
	/// @return A rotation quaternion.
	glm::quat getRotation();

	/// @brief Returns the scale of the transform.
	/// @return A 3D scale vector.
	glm::vec3 getScale();

	/// @brief Returns the vector going into the forward direction of the transform.
	/// @return A 3D directional vector.
	glm::vec3 getForward();

	/// @brief Returns the vector going into the up direction of the transform.
	/// @return A 3D directional vector.
	glm::vec3 getUp();

	/// @brief Returns the vector going into the right direction of the transform.
	/// @return A 3D directional vector.
	glm::vec3 getRight();

	/// @brief Returns the model matrix of the transform.
	/// @return A 4x4 model matrix.
	glm::mat4x4 getModelMatrix();

	// ========== Setters ==========
	
	/// @brief Sets the position of the transform.
	/// @param position The new position of the transform.
	void setPosition(glm::vec3 position);

	/// @brief Sets the rotation of the transform.
	/// @param rotation The new rotation as a quaternion.
	void setRotation(glm::quat rotation);

	/// @brief Sets the rotation of the transform.
	/// @param rotation The new rotation as a 3D vector containing euler angles.
	void setRotation(glm::vec3 eulerAngles);

	/// @brief Sets the scale of the transform.
	/// @param scale The new scale as a 3D vector.
	void setScale(glm::vec3 scale);

	// ========== Transform Operations ==========
	
	/// @brief Resets the transform.
	void reset();

	/// @brief Translates the transform by a given translation vector.
	/// @param translationVector Vector by which the transform is to be translated.
	void translate(glm::vec3 translationVector);

	/// @brief Rotates the transform by a given rotation vector.
	/// @param eulerAngles Euler angles by which the transform should be rotated.
	void rotate(glm::vec3 eulerAngles);

	/// @brief Rotates the transform by a given angle around an axis.
	/// @param rotationAxis Axis to rotate around.
	/// @param angle Rotation angle expressed in radians.
	void rotate(glm::vec3 rotationAxis, float angle);

	/// @brief Rotates the transform in world space.
	/// @param eulerAngles Euler angles by which to rotate.
	void rotateGlobally(glm::vec3 eulerAngles);
	
	void lookAt();
};