#include"Quaternion.h"

// ========== Constructors ==========

Quaternion::Quaternion() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->r = 1;
}

Quaternion::Quaternion(glm::vec3 eulerAngles) {
	Quaternion x = Quaternion(glm::vec3(1, 0, 0), eulerAngles.x);
	Quaternion y = Quaternion(glm::vec3(0, 1, 0), eulerAngles.y);
	Quaternion z = Quaternion(glm::vec3(0, 0, 1), eulerAngles.z);
	Quaternion result = y*x*z;
	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
	this->r = result.r;
}

Quaternion::Quaternion(glm::vec3 axis, double angle) {
	float radAngle = glm::radians(angle / 2.0f);
	this->r = cos(radAngle);
	this->x = axis.x * sin(radAngle);
	this->y = axis.y * sin(radAngle);
	this->z = axis.z * sin(radAngle);
}

Quaternion::Quaternion(float x, float y, float z, float r) {
	this->r = r;
	this->x = x;
	this->y = y;
	this->z = z;
}

// ========== Getters ==========

glm::vec4 Quaternion::getAsVector() {
	return glm::vec4(x, y, z, r);
}

glm::vec3 Quaternion::getEulerAngles() {
	float t0 = +2.0 * (this->r * this->x + this->y * this->z);
	float	t1 = +1.0 - 2.0 * (this->x * this->x + this->y * this->y);
	float	roll_x = atan2(t0, t1);

	float	t2 = +2.0 * (this->r * this->y - this->z * this->x);
	t2 = t2 > +1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	float	pitch_y = asin(t2);

	float	t3 = +2.0 * (this->r * this->z + this->x * this->y);
	float	t4 = +1.0 - 2.0 * (this->y * this->y + this->z * this->z);
	float	yaw_z = atan2(t3, t4);

	//return roll_x, pitch_y, yaw_z # in radians
	return glm::vec3(glm::degrees(roll_x), glm::degrees(pitch_y), glm::degrees(yaw_z));
}

glm::vec3 Quaternion::getAxis() {
	glm::vec3 axis;
	if (getAngle() == 0) {
		axis = glm::vec3(0, 1, 0);
	}
	else if (abs(getAngle()) == 180) {
		axis = glm::vec3(x, y, z);
	}
	else {
		axis = glm::vec3(this->x / sqrt(1.0f - this->r * this->r),
			this->y / sqrt(1.0f - this->r * this->r),
			this->z / sqrt(1.0f - this->r * this->r));
	}
	axis = glm::vec3(0, 1, 0);
	return axis;
}

float Quaternion::getAngle() {
	return 2.0f * acos(this->r);
}

glm::mat4 Quaternion::getMatrix() {
	glm::mat4 mat1 = {
		r, z, -y, x,
		-z, r, x, y,
		y, -x, r, z,
		-x, -y, -z, r
	};
	glm::mat4 mat2 = {
		r, z, -y, -x,
		-z, r, x, -y,
		y, -x, r, -z,
		x, y, z, r
	};

	return mat2 * mat1;
}

Quaternion Quaternion::getConjugated() {
	return Quaternion(-x, -y, -z, r);
}

// ========== Setters ==========

void Quaternion::setComponents(glm::vec4 components) {
	x = components.x;
	y = components.y;
	z = components.z;
	r = components.w;
}

// ========== Quaternion Functions ==========

void Quaternion::rotate(glm::vec3 angles) {
	Quaternion b = Quaternion(angles);
	Quaternion result;
	result.r = b.r * this->r - b.x * this->x - b.y * this->y - b.z * this->z;
	result.x = b.r * this->x + b.x * this->r + b.y * this->z - b.z * this->y;
	result.y = b.r * this->y - b.x * this->z + b.y * this->r + b.z * this->x;
	result.z = b.r * this->z + b.x * this->y - b.y * this->x + b.z * this->r;
	
	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
	this->r = result.r;
}

void Quaternion::rotate(double xAngle, double yAngle, double zAngle) {
	Quaternion b = Quaternion(glm::vec3(xAngle, yAngle, zAngle));
	Quaternion result;
	result.r = this->r * b.r - this->x * b.x - this->y * b.y - this->z * b.z;
	result.x = this->r * b.x + this->x * b.r + this->y * b.z - this->z * b.y;
	result.y = this->r * b.y - this->x * b.z + this->y * b.r + this->z * b.x;
	result.z = this->r * b.z + this->x * b.y - this->y * b.x + this->z * b.r;


	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
	this->r = result.r;
}

void Quaternion::rotate(glm::vec3 axis, double angle) {
	Quaternion b = Quaternion(axis, angle);
	Quaternion result;

	result.r = this->r * b.r - this->x * b.x - this->y * b.y - this->z * b.z;
	result.x = this->r * b.x + this->x * b.r + this->y * b.z - this->z * b.y;
	result.y = this->r * b.y - this->x * b.z + this->y * b.r + this->z * b.x;
	result.z = this->r * b.z + this->x * b.y - this->y * b.x + this->z * b.r;
	
	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
	this->r = result.r;
}