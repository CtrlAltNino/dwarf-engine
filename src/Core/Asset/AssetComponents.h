#pragma once

#include <nlohmann/json.hpp>

#include "Utilities/FileHandler.h"
#include "Core/Base.h"
#include "Core/Rendering/Mesh.h"
#include "Core/Rendering/TextureCreator.h"
#include "Core/Rendering/Material.h"
#include "Core/Asset/ModelImporter.h"
#include "Core/Asset/MaterialSerializer.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf
{
  /// @brief Component containing a model asset.
  struct ModelAsset
  {
    /// @brief Vector of submeshes.
    std::vector<std::shared_ptr<Mesh>> m_Meshes;

    explicit ModelAsset(std::filesystem::path const& path)
      : m_Meshes(ModelImporter::Import(path))
    {
    }
    void
    Load() const
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
    std::shared_ptr<Material> m_Material;

    explicit MaterialAsset(std::filesystem::path const& path)
    {
      // Use Mesh Importer with meta data to import mesh
      m_Material = MaterialSerializer::Deserialize(path);
      m_Material->GetShader()->Compile();
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
  struct TesselationControlShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit TesselationControlShaderAsset(std::filesystem::path const& path)
      : m_Path(path)
      , m_FileContent(FileHandler::ReadFile(m_Path))
    {
    }
  };

  /// @brief Component containing a tesselation evaluation shader asset.
  struct TesselationEvaluationShaderAsset
  {
    /// @brief Path to the asset.
    std::filesystem::path m_Path;
    std::string           m_FileContent;

    explicit TesselationEvaluationShaderAsset(std::filesystem::path const& path)
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
    std::shared_ptr<Texture> m_Texture;

    explicit TextureAsset(std::filesystem::path const& path)
      : m_Texture(TextureCreator::FromPath(path))
    {
    }

    /// @brief Retrieves the texture.
    /// @return The imported texture instance.
    std::shared_ptr<Texture>
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