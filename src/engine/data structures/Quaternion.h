#pragma once

//#include<glad/glad.h>
#include<glm/glm.hpp>

class Quaternion {
private:
	float x, y, z, r;
public:
	// ========== Constructors ==========
	
	Quaternion();
	Quaternion(glm::vec3 eulerAngles);
	Quaternion(glm::vec3 axis, double angle);
	Quaternion(float x, float y, float z, float r);

	// ========== Getters ==========
	
	glm::vec4 getAsVector();
	glm::vec3 getEulerAngles();
	glm::vec3 getAxis();
	float getAngle();
	glm::mat4 getMatrix();
	Quaternion getConjugated();

	// ========== Setters ==========
	
	void setComponents(glm::vec4 components);
	
	// ========== Quaternion Functions ==========
	
	void rotate(glm::vec3 angles);
	void rotate(double xAngle, double yAngle, double zAngle);
	void rotate(glm::vec3 axis, double angle);

	// ========== Static Quaternion Functions ==========

	static Quaternion fromEulerAngles(glm::vec3 eulerAngles) {
		return Quaternion(eulerAngles);
	}

	static Quaternion fromEulerAngles(float xAngle, float yAngle, float zAngle) {
		return Quaternion(glm::vec3(xAngle, yAngle, zAngle));
	}
	
	static Quaternion getIdentity() {
		return Quaternion(0, 0, 0, 1);
	}
	
	// ========== Quaternion Operators ==========

	Quaternion operator*(const Quaternion& b) {
		Quaternion result;
		/*glm::vec3 vB = glm::vec3(b.x, b.y, b.z);
		glm::vec3 vA = glm::vec3(this->x, this->y, this->z);
		result.r = b.r * this->r - glm::dot(vB, vA);
		glm::vec3 v = (float)b.r * vA + (float)this->r * vB + glm::cross(vA, vB);
		result.x = v.x;
		result.y = v.y;
		result.z = v.z;*/

		result.r = this->r * b.r - this->x * b.x - this->y * b.y - this->z * b.z;
		result.x = this->r * b.x + this->x * b.r + this->y * b.z - this->z * b.y;
		result.y = this->r * b.y - this->x * b.z + this->y * b.r + this->z * b.x;
		result.z = this->r * b.z + this->x * b.y - this->y * b.x + this->z * b.r;

		return result;
	}

	glm::vec3 operator*(const glm::vec3& b) {
		Quaternion result = getConjugated() * Quaternion(b.x, b.y, b.z, 0) * Quaternion(x, y, z, r);

		return glm::vec3(result.x, result.y, result.z);
	}
};