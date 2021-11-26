#ifndef VERTEX_CLASS_H
#define VERTEX_CLASS_H

#include<glm/glm.hpp>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec2 uv;
};

#endif