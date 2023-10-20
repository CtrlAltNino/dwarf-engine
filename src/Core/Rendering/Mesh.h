#pragma once

#include "Core/Base.h"
#include "Vertex.h"
#include <filesystem>

namespace Dwarf
{
    class Mesh
    {
    protected:
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        unsigned int m_MaterialIndex;

    public:
        static Ref<Mesh> Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex);
        static void Init();

        virtual void SetupMesh() = 0;

        int GetMaterialIndex()
        {
            return m_MaterialIndex;
        }

        int m_VertexCount;
        int m_IndexCount;
        static Ref<Mesh> s_GridMesh;
    };
}