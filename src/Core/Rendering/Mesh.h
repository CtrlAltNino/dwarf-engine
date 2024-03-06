#pragma once

#include "Core/Base.h"
#include "Vertex.h"
#include <filesystem>

namespace Dwarf
{
    class Mesh
    {
    private:
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        unsigned int m_MaterialIndex;

    public:
        Mesh(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, unsigned int materialIndex);
        virtual ~Mesh() = default;

        static Ref<Mesh> Create(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, unsigned int materialIndex);
        static Ref<Mesh> GenerateUnitSphere(int stacks, int slices);
        static void Init();

        virtual void SetupMesh() = 0;

        int GetMaterialIndex() const;
        const std::vector<Vertex> &GetVertices() const;
        const std::vector<unsigned int> &GetIndices() const;

        static Ref<Mesh> s_GridMesh;
        static Ref<Mesh> s_UnitSphere;
    };
}