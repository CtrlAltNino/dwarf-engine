#pragma once

#include<glm/glm.hpp>

/// @brief Structure that stores information of a vertex.
struct Vertex {
	/// @brief Position of the vertex.
	glm::vec3 pos;

	/// @brief Normal of the vertex.
	glm::vec3 normal;

	/// @brief Tangent of the vertex.
	glm::vec3 tangent;

	/// @brief Bi-Tangent of the vertex.
	glm::vec3 biTangent;

	/// @brief The UV of the vertex.
	glm::vec2 uv;
};