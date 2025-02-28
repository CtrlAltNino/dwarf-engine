#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dwarf
{
  glm::mat4
  AssimpToGlmMatrix(const aiMatrix4x4& mat)
  {
    return glm::mat4(mat.a1,
                     mat.b1,
                     mat.c1,
                     mat.d1,
                     mat.a2,
                     mat.b2,
                     mat.c2,
                     mat.d2,
                     mat.a3,
                     mat.b3,
                     mat.c3,
                     mat.d3,
                     mat.a4,
                     mat.b4,
                     mat.c4,
                     mat.d4);
  }

  ModelImporter::ModelImporter(std::shared_ptr<IDwarfLogger>   logger,
                               std::shared_ptr<IAssetMetadata> assetMetadata,
                               std::shared_ptr<IMeshFactory>   meshFactory)
    : m_Logger(logger)
    , m_AssetMetadata(assetMetadata)
    , m_MeshFactory(meshFactory)
  {
    m_Logger->LogDebug(Log("Creating ModelImporter", "ModelImporter"));
  }

  // @brief Imports a model.
  /// @param path Path to the model.
  /// @return List of the imported meshes of a model.
  std::vector<std::unique_ptr<IMesh>>
  ModelImporter::Import(const std::filesystem::path& path)
  {
    m_Logger->LogDebug(Log(
      fmt::format("Importing model from {}", path.string()), "ModelImporter"));
    nlohmann::json metaData = m_AssetMetadata->GetMetadata(path);

    m_Logger->LogDebug(
      Log(fmt::format("Metadata:\n{}", metaData.dump(2)), "ModelImporter"));

    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(
      path.string(),
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
      m_Logger->LogError(
        Log(fmt::format("Assimp Error: {}", importer.GetErrorString()),
            "ModelImporter"));
      return std::vector<std::unique_ptr<IMesh>>();
    }

    aiMatrix4x4 modelTransform; // Create a matrix for transformation

    if (scene->mMetaData)
    {
      aiString upAxis;
      scene->mMetaData->Get("UpAxis", upAxis);

      // std::cout << "Has metadata" << std::endl;
      // std::cout << "UpAxis: " << upAxis.C_Str() << std::endl;

      if (upAxis == aiString("Z"))
      {
        // Rotate by -90 degrees on the X-axis if the model is Z-up
        aiMatrix4x4::RotationX(-AI_MATH_PI / 2, modelTransform);
      }
      else if (upAxis == aiString("X"))
      {
        // Rotate accordingly for X-up models
        aiMatrix4x4::RotationZ(AI_MATH_PI / 2, modelTransform);
      }
      // Apply modelTransform to the scene or node’s transform here.
    }

    std::vector<std::unique_ptr<IMesh>> meshes;

    // Rotate scene->mRootNode->mTransformation to match the coordinate system
    // of the engine

    /*aiMatrix4x4 mat;
    aiMatrix4x4::RotationX(-glm::half_pi<float>(),
                           mat); // Rotate 90 degrees around the X-axis

    scene->mRootNode->mTransformation = mat *
    scene->mRootNode->mTransformation;*/

    ModelImporter::ProcessNode(
      scene->mRootNode,
      scene,
      meshes,
      AssimpToGlmMatrix(scene->mRootNode->mTransformation));

    return std::move(meshes);
  }

  void
  ModelImporter::ProcessNode(const aiNode*                        node,
                             const aiScene*                       scene,
                             std::vector<std::unique_ptr<IMesh>>& meshes,
                             glm::mat4 parentTransform)
  {
    glm::mat4 nodeTransform = AssimpToGlmMatrix(node->mTransformation);
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      // meshes.push_back(ProcessMesh(mesh, scene));
      ProcessMesh(mesh, scene, meshes, globalTransform);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
      // std::vector<std::unique_ptr<IMeshBuffer>> recursedMeshes =
      ProcessNode(node->mChildren[i], scene, meshes, globalTransform);
      // join the meshes
      // TODO: Check if this is the correct way to join the meshes
      // meshes.insert(meshes.end(),
      //              std::make_move_iterator(recursedMeshes.begin()),
      //              std::make_move_iterator(recursedMeshes.end()));
    }
  }

  void
  ModelImporter::ProcessMesh(const aiMesh*                        mesh,
                             const aiScene*                       scene,
                             std::vector<std::unique_ptr<IMesh>>& meshes,
                             glm::mat4                            transform)
  {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int              materialIndex = mesh->mMaterialIndex;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;

      // Convert the Assimp vertex position to a GLM vector
      glm::vec4 position = glm::vec4(mesh->mVertices[i].x,
                                     mesh->mVertices[i].y,
                                     mesh->mVertices[i].z,
                                     1.0f // Homogeneous coordinate
      );

      // Apply the transformation
      position = transform * position;

      // process vertex positions, normals and texture coordinates
      vertex.Position = glm::vec3(position);

      if (mesh->HasNormals())
      {
        glm::vec4 normal = glm::vec4(mesh->mNormals[i].x,
                                     mesh->mNormals[i].y,
                                     mesh->mNormals[i].z,
                                     0.0f // W=0 to ignore translation
        );
        normal = transform * normal;

        vertex.Normal = glm::normalize(glm::vec3(normal));
      }

      if (mesh->mTextureCoords[0])
      {
        vertex.UV =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }

      if (mesh->HasTangentsAndBitangents())
      {
        glm::vec4 tangent = glm::vec4(mesh->mTangents[i].x,
                                      mesh->mTangents[i].y,
                                      mesh->mTangents[i].z,
                                      0.0f // W=0 to ignore translation
        );
        glm::vec4 biTangent = glm::vec4(mesh->mBitangents[i].x,
                                        mesh->mBitangents[i].y,
                                        mesh->mBitangents[i].z,
                                        0.0f // W=0 to ignore translation
        );

        tangent = transform * tangent;
        biTangent = transform * biTangent;

        vertex.Tangent = glm::vec3(tangent);
        vertex.BiTangent = glm::vec3(biTangent);
      }

      vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    meshes.push_back(
      std::move(m_MeshFactory->Create(vertices, indices, materialIndex)));
  }
}