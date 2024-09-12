#pragma once
#include "Core/Rendering/Texture/ITexture.h"
#include "Utilities/FileHandler.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Material/IMaterial.h"

namespace Dwarf
{
  /// @brief Component containing a model asset.
  struct ModelAsset
  {
  private:
    /// @brief Vector of submeshes.
    std::vector<std::unique_ptr<IMesh>> m_Meshes;

  public:
    explicit ModelAsset(std::vector<std::unique_ptr<IMesh>>& meshes)
      : m_Meshes(std::move(meshes))
    {
      for (auto& mesh : m_Meshes)
      {
        mesh->SetupMesh();
      }
    }

    std::vector<std::unique_ptr<IMesh>>&
    GetMeshes()
    {
      return m_Meshes;
    }
  };

  /// @brief Component containing a material asset.
  struct MaterialAsset
  {
  private:
    /// @brief Imported material.
    std::unique_ptr<IMaterial> m_Material;

  public:
    explicit MaterialAsset(std::unique_ptr<IMaterial> material)
    {
      // Use Mesh Importer with meta data to import mesh
      // m_Material = MaterialSerializer::Deserialize(path);
      // m_Material->GetShader()->Compile();
    }

    IMaterial&
    GetMaterial()
    {
      return *m_Material;
    }
  };

  /// @brief Component containing a vertex shader asset.
  struct VertexShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit VertexShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a fragment shader asset.
  struct FragmentShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit FragmentShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a geometry shader asset.
  struct GeometryShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit GeometryShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a tesselation control shader asset.
  struct TessellationControlShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit TessellationControlShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a tesselation evaluation shader asset.
  struct TessellationEvaluationShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit TessellationEvaluationShaderAsset(
      std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a compute shader asset.
  struct ComputeShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit ComputeShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a compute shader asset.
  struct HlslShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit HlslShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a texture asset.
  struct TextureAsset
  {
    /// @brief Imported texture.
    std::unique_ptr<ITexture> m_Texture;

    explicit TextureAsset(std::unique_ptr<ITexture> texture)
      : m_Texture(std::move(texture))
    {
      std::cout << "Texture Asset created" << std::endl;
      std::cout << "Texture ID: " << m_Texture->GetTextureID() << std::endl;
    }
  };

  /// @brief Asset containing a Dwarf Engine scene.
  struct SceneAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;

    explicit SceneAsset(std::filesystem::path const& path)
      : m_Path(path)
    {
    }
  };

  struct UnknownAsset
  {
    std::filesystem::path m_Path;
    std::string           m_FileContent;
    explicit UnknownAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };
}