#include "pch.h"

#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Dwarf
{
  auto
  AssimpToGlmMatrix(const aiMatrix4x4& from) -> glm::mat4
  {
    glm::mat4 to;

    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;

    return to;
  }

  auto
  GetImportFlags(const std::filesystem::path& path) -> unsigned int
  {
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                         aiProcess_CalcTangentSpace |
                         aiProcess_JoinIdenticalVertices;
    std::string ext = path.extension().string();

    if (ext == ".obj")
    {
      flags |= aiProcess_FlipUVs; // OBJ is already OpenGL-friendly
    }
    else if (ext == ".fbx" || ext == ".gltf" || ext == ".dae")
    {
      flags |= aiProcess_FlipUVs; // Ensure face culling works
    }
    else if (ext == ".3ds")
    {
      flags |= aiProcess_MakeLeftHanded |
               aiProcess_FlipUVs; // Convert to right-handed OpenGL
    }

    return flags;
  }

  ModelImporter::ModelImporter(std::shared_ptr<IDwarfLogger>   logger,
                               std::shared_ptr<IAssetMetadata> assetMetadata,
                               std::shared_ptr<IMeshFactory>   meshFactory)
    : mLogger(std::move(logger))
    , mAssetMetadata(std::move(assetMetadata))
    , mMeshFactory(std::move(meshFactory))
  {
    mLogger->LogDebug(Log("Creating ModelImporter", "ModelImporter"));
  }

  auto
  ModelImporter::Import(const std::filesystem::path& path)
    -> std::vector<std::unique_ptr<IMesh>>
  {
    mLogger->LogDebug(Log(fmt::format("Importing model from {}", path.string()),
                          "ModelImporter"));
    nlohmann::json metaData = mAssetMetadata->GetMetadata(path);

    mLogger->LogDebug(
      Log(fmt::format("Metadata:\n{}", metaData.dump(2)), "ModelImporter"));

    Assimp::Importer importer;
    const aiScene*   scene =
      importer.ReadFile(path.string(), GetImportFlags(path));

    if ((scene == nullptr) ||
        ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U) ||
        (scene->mRootNode == nullptr))
    {
      mLogger->LogError(
        Log(fmt::format("Assimp Error: {}", importer.GetErrorString()),
            "ModelImporter"));
      return {};
    }

    aiMatrix4x4 modelTransform; // Create a matrix for transformation

    if (scene->mMetaData != nullptr)
    {
      aiString upAxis;
      scene->mMetaData->Get("UpAxis", upAxis);

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

    ModelImporter::ProcessNode(
      scene->mRootNode, scene, meshes, glm::mat4(1.0F));

    return meshes;
  }

  void
  ModelImporter::ProcessNode(const aiNode*                        node,
                             const aiScene*                       scene,
                             std::vector<std::unique_ptr<IMesh>>& meshes,
                             glm::mat4 parentTransform)
  {
    glm::mat4          nodeTransform = AssimpToGlmMatrix(node->mTransformation);
    glm::mat4          globalTransform = parentTransform * nodeTransform;
    std::span<aiMesh*> meshSpan(scene->mMeshes, scene->mNumMeshes);
    std::span<uint32_t> nodeMeshesSpan(node->mMeshes, node->mNumMeshes);

    // process all the node's meshes (if any)
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
      const aiMesh* mesh = meshSpan[nodeMeshesSpan[i]];
      // meshes.push_back(ProcessMesh(mesh, scene));
      ProcessMesh(mesh, meshes, globalTransform);
    }

    std::span<aiNode*> nodeChildrenSpan(node->mChildren, node->mNumChildren);
    // then do the same for each of its children
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
      ProcessNode(nodeChildrenSpan[i], scene, meshes, globalTransform);
    }
  }

  void
  ModelImporter::ProcessMesh(const aiMesh*                        mesh,
                             std::vector<std::unique_ptr<IMesh>>& meshes,
                             glm::mat4                            transform)
  {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    uint32_t              materialIndex = mesh->mMaterialIndex;
    std::span<aiVector3D> meshVerticesSpan(mesh->mVertices, mesh->mNumVertices);
    std::span<aiVector3D> meshNormalsSpan(mesh->mNormals, mesh->mNumVertices);
    std::span<aiVector3D> meshTangentsSpan(mesh->mTangents, mesh->mNumVertices);
    std::span<aiVector3D> meshBitangentsSpan(mesh->mBitangents,
                                             mesh->mNumVertices);
    std::span<aiVector3D* const, AI_MAX_NUMBER_OF_TEXTURECOORDS>
                      meshTexCoordsSpan(mesh->mTextureCoords);
    std::span<aiFace> meshFacesSpan(mesh->mFaces, mesh->mNumFaces);

    for (uint32_t i = 0; i < meshVerticesSpan.size(); i++)
    {
      Vertex vertex;

      // Convert the Assimp vertex position to a GLM vector
      glm::vec4 position = glm::vec4(meshVerticesSpan[i].x,
                                     meshVerticesSpan[i].y,
                                     meshVerticesSpan[i].z,
                                     1.0F // Homogeneous coordinate
      );

      // Apply the transformation
      position = transform * position;

      // process vertex positions, normals and texture coordinates
      vertex.Position = glm::vec3(position);

      if (mesh->HasNormals())
      {
        glm::vec4 normal = glm::vec4(meshNormalsSpan[i].x,
                                     meshNormalsSpan[i].y,
                                     meshNormalsSpan[i].z,
                                     0.0F // W=0 to ignore translation
        );
        normal = transform * normal;

        vertex.Normal = glm::normalize(glm::vec3(normal));
      }

      if (mesh->mTextureCoords[0] != nullptr)
      {
        vertex.UV =
          glm::vec2(meshTexCoordsSpan[0][i].x, meshTexCoordsSpan[0][i].y);
      }

      if (mesh->HasTangentsAndBitangents())
      {
        glm::vec4 tangent = glm::vec4(meshTangentsSpan[i].x,
                                      meshTangentsSpan[i].y,
                                      meshTangentsSpan[i].z,
                                      0.0F // W=0 to ignore translation
        );
        glm::vec4 biTangent = glm::vec4(meshBitangentsSpan[i].x,
                                        meshBitangentsSpan[i].y,
                                        meshBitangentsSpan[i].z,
                                        0.0F // W=0 to ignore translation
        );

        tangent = transform * tangent;
        biTangent = transform * biTangent;

        vertex.Tangent = glm::vec3(tangent);
        vertex.BiTangent = glm::vec3(biTangent);
      }

      vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
      const aiFace&       face = meshFacesSpan[i];
      std::span<uint32_t> faceIndicesSpan(face.mIndices, face.mNumIndices);
      for (uint32_t j = 0; j < face.mNumIndices; j++)
      {
        indices.push_back(faceIndicesSpan[j]);
      }
    }

    meshes.push_back(
      std::move(mMeshFactory->Create(vertices, indices, materialIndex)));
  }
}