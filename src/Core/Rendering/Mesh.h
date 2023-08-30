#pragma once

#include "Core/Base.h"
#include "Vertex.h"
#include <filesystem>

namespace Dwarf {
    class Mesh {
        private:
            std::vector<Vertex> m_Vertices;
            std::vector<int> m_Indices;
            int m_MaterialIndex;

        public:
            static Ref<Mesh> Create(std::filesystem::path path);

            virtual void GenerateBuffers() = 0;

            /*void SetVertices();

            void SetIndices();

            void SetMaterialIndex();

            std::vector<Vertex> GetVertices();

            std::vector<int> GetIndices();*/

            int GetMaterialIndex();
    };
}