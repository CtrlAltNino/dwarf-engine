#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "Utilities/FileHandler.h"

namespace Dwarf {

	/// @brief Class representating a texture in OpenGL
	class Texture {
		public:
			/// @brief The OpenGL texture handle.
			GLuint ID;

			/// @brief The size of the texture.
			glm::ivec2 size;
			Texture() = default;
			Texture(const char* filePath);
			Texture(const char* filePath, GLint filter, GLint wrap, GLint colorFormat, GLint dataFormat);

			/// @brief Binds the texture.
			void Bind();

			/// @brief Unbinds the texture.
			void Unbind();

			/// @brief Deletes the texture.
			void Delete();

			/// @brief Retrieves the size of the texture.
			/// @return The size of the texture as a 2D vector.
			glm::ivec2 GetTextureSize();
	};
}