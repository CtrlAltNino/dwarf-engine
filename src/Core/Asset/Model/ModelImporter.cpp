#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dwarf
{
  ModelImporter::ModelImporter(std::shared_ptr<IAssetMetadata> assetMetadata,
                               std::shared_ptr<IMeshFactory>   meshFactory)
    : m_AssetMetadata(assetMetadata)
    , m_MeshFactory(meshFactory)
  {
  }
  // @brief Imports a model.
  /// @param path Path to the model.
  /// @return List of the imported meshes of a model.
  std::vector<std::shared_ptr<IMesh>>
  ModelImporter::Import(std::filesystem::path const& path)
  {
    nlohmann::json metaData = m_AssetMetadata->GetMetadata(path);

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(
      path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
      std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return std::vector<std::shared_ptr<IMesh>>();
    }

    return ModelImporter::ProcessNode(scene->mRootNode, scene);
  }

  std::vector<std::shared_ptr<IMesh>>
  ModelImporter::ProcessNode(const aiNode* node, const aiScene* scene)
  {
    std::vector<std::shared_ptr<IMesh>> meshes;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(ModelImporter::ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
      std::vector<std::shared_ptr<IMesh>> joinedMeshes;
      std::vector<std::shared_ptr<IMesh>> recursedMeshes =
        ModelImporter::ProcessNode(node->mChildren[i], scene);
      joinedMeshes.reserve(meshes.size() + meshes.size()); // preallocate memory
      joinedMeshes.insert(joinedMeshes.end(), meshes.begin(), meshes.end());
      joinedMeshes.insert(
        joinedMeshes.end(), recursedMeshes.begin(), recursedMeshes.end());
      meshes = joinedMeshes;
    }

    return meshes;
  }

  std::shared_ptr<IMesh>
  ModelImporter::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
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
        vertex.UV =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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

    return m_MeshFactory->CreateMesh(vertices, indices, materialIndex);
  }
}