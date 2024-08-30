#pragma once

#include "pch.h"

namespace Dwarf
{
  /// @brief Structure that stores information of a vertex.
  struct Vertex
  {
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

    Vertex(glm::vec3 position,
           glm::vec3 normal,
           glm::vec3 tangent,
           glm::vec3 biTangent,
           glm::vec2 uv)
      : Position(position)
      , Normal(normal)
      , Tangent(tangent)
      , BiTangent(biTangent)
      , UV(uv)
    {
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
      : Position(position)
      , Normal(normal)
      , UV(uv)
    {
    }

    Vertex();
  };
}