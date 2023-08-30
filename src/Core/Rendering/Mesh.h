#pragma once

#include "Core/Base.h"
#include "Vertex.h"
#include <filesystem>

namespace Dwarf {
    class Mesh {
        protected:
            std::vector<Vertex> m_Vertices;
            std::vector<unsigned int> m_Indices;
            unsigned int m_MaterialIndex;

        public:
            static Ref<Mesh> Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex);

            virtual void SetupMesh() = 0;

            int GetMaterialIndex();
    };
}