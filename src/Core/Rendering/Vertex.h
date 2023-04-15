#pragma once

#include <glm/glm.hpp>

namespace Dwarf {

	/// @brief Structure that stores information of a vertex.
	struct Vertex {
		/// @brief Position of the vertex.
		glm::vec3 Position;

		/// @brief Normal of the vertex.
		glm::vec3 Normal;

		/// @brief Tangent of the vertex.
		glm::vec3 Tangent;

		/// @brief Bi-Tangent of the vertex.
		glm::vec3 BiTangent;

		/// @brief The UV of the vertex.
		glm::vec2 UV;
	};
}