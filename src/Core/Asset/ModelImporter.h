#pragma once

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Rendering/Mesh.h"
#include "Core/Asset/AssetMetaData.h"
#include "Core/Rendering/Vertex.h"

namespace Dwarf
{

  /// @brief Utilities for importing models.
  class ModelImporter
  {
  public:
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    static std::vector<Ref<Mesh>>
    Import(std::filesystem::path const& path)
    {
      nlohmann::json metaData = AssetMetaData::GetMetaData(path);

      Assimp::Importer importer;
      const aiScene*   scene = importer.ReadFile(
        path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
          !scene->mRootNode)
      {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString()
                  << std::endl;
        return std::vector<Ref<Mesh>>();
      }

      return ModelImporter::ProcessNode(scene->mRootNode, scene);
    }

  private:
    static std::vector<Ref<Mesh>>
    ProcessNode(const aiNode* node, const aiScene* scene)
    {
      std::vector<Ref<Mesh>> meshes;

      // process all the node's meshes (if any)
      for (unsigned int i = 0; i < node->mNumMeshes; i++)
      {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ModelImporter::ProcessMesh(mesh, scene));
      }
      // then do the same for each of its children
      for (unsigned int i = 0; i < node->mNumChildren; i++)
      {
        std::vector<Ref<Mesh>> joinedMeshes;
        std::vector<Ref<Mesh>> recursedMeshes =
          ModelImporter::ProcessNode(node->mChildren[i], scene);
        joinedMeshes.reserve(meshes.size() +
                             meshes.size()); // preallocate memory
        joinedMeshes.insert(joinedMeshes.end(), meshes.begin(), meshes.end());
        joinedMeshes.insert(
          joinedMeshes.end(), recursedMeshes.begin(), recursedMeshes.end());
        meshes = joinedMeshes;
      }

      return meshes;
    }

    static Ref<Mesh>
    ProcessMesh(const aiMesh* mesh, const aiScene* scene)
    {
      std::vector<Vertex>       vertices;
      std::vector<unsigned int> indices;
      unsigned int              materialIndex = mesh->mMaterialIndex;

      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.Position = glm::vec3(
          mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(
          mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
        {
          vertex.UV = glm::vec2(mesh->mTextureCoords[0][i].x,
                                mesh->mTextureCoords[0][i].y);
        }

        if (mesh->mTangents)
        {
          vertex.Tangent = glm::vec3(
            mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }

        if (mesh->mBitangents)
        {
          vertex.BiTangent = glm::vec3(mesh->mBitangents[i].x,
                                       mesh->mBitangents[i].y,
                                       mesh->mBitangents[i].z);
        }

        vertices.push_back(vertex);
      }

      for (unsigned int i = 0; i < mesh->mNumFaces; i++)
      {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
          indices.push_back(face.mIndices[j]);
      }

      return Mesh::Create(vertices, indices, materialIndex);
    }
  };
}