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
    /// @brief Vector of submeshes.
    std::vector<std::shared_ptr<IMesh>> m_Meshes;

    explicit ModelAsset(std::vector<std::shared_ptr<IMesh>> meshes)
      : m_Meshes(meshes)
    {
      for (const auto& mesh : m_Meshes)
      {
        mesh->SetupMesh();
      }
    }
  };

  /// @brief Component containing a material asset.
  struct MaterialAsset
  {
    /// @brief Imported material.
    std::shared_ptr<IMaterial> m_Material;

    explicit MaterialAsset(std::shared_ptr<IMaterial> material)
    {
      // Use Mesh Importer with meta data to import mesh
      // m_Material = MaterialSerializer::Deserialize(path);
      // m_Material->GetShader()->Compile();
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
    std::shared_ptr<ITexture> m_Texture;

    explicit TextureAsset(std::shared_ptr<ITexture> const& texture)
      : m_Texture(texture)
    {
    }

    /// @brief Retrieves the texture.
    /// @return The imported texture instance.
    std::shared_ptr<ITexture>
    Get() const
    {
      return m_Texture;
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