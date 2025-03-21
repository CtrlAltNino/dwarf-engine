#pragma once

#include "pch.h"

namespace Dwarf
{
  /// @brief Structure that stores information of a vertex.
  struct Vertex
  {
    /// @brief Position of the vertex.
    glm::vec3 Position = glm::vec3(0, 0, 0);

    /// @brief Normal of the vertex.
    glm::vec3 Normal = glm::vec3(0, 0, 0);

    /// @brief Tangent of the vertex.
    glm::vec3 Tangent = glm::vec3(0, 0, 0);

    /// @brief Bi-Tangent of the vertex.
    glm::vec3 BiTangent = glm::vec3(0, 0, 0);

    /// @brief The UV of the vertex.
    glm::vec2 UV = glm::vec3(0, 0, 0);

    Vertex(glm::vec3 position,
           glm::vec3 normal,
           glm::vec3 tangent,
           glm::vec3 biTangent,
           glm::vec2 uvCoord)
      : Position(position)
      , Normal(normal)
      , Tangent(tangent)
      , BiTangent(biTangent)
      , UV(uvCoord)
    {
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
      : Position(position)
      , Normal(normal)
      , UV(uv)
    {
    }

    Vertex() {};
  };
}