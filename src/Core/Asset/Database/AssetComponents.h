#pragma once

#include "Core/Rendering/Texture/ITexture.h"
#include "Utilities/FileHandler.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Material/IMaterial.h"
#include <optional>

namespace Dwarf
{
  /// @brief Component containing a model asset.
  struct ModelAsset
  {
  private:
    /// @brief Vector of submeshes.
    std::vector<IMesh> m_Meshes;

  public:
    ModelAsset() = default;
    ModelAsset(std::vector<IMesh> meshes)
      : m_Meshes(std::move(meshes))
    {
      // for (auto& mesh : m_Meshes)
      // {
      //   mesh.SetupMesh();
      // }
    }

    std::vector<IMesh>&
    Meshes()
    {
      return m_Meshes;
    }

    const std::vector<IMesh>&
    Meshes() const
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
    MaterialAsset(std::unique_ptr<IMaterial>&& material)
      : m_Material(std::move(material))
    {
    }

    IMaterial&
    GetMaterial()
    {
      return *m_Material;
    }

    const IMaterial&
    GetMaterial() const
    {
      return *m_Material;
    }
  };

  /// @brief Component containing a vertex shader asset.
  struct VertexShaderAsset
  {
  private:
    /// @brief The content of the file.
    std::string m_FileContent;

  public:
    explicit VertexShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a fragment shader asset.
  struct FragmentShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit FragmentShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a geometry shader asset.
  struct GeometryShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit GeometryShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a tesselation control shader asset.
  struct TessellationControlShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit TessellationControlShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a tesselation evaluation shader asset.
  struct TessellationEvaluationShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit TessellationEvaluationShaderAsset(
      const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct ComputeShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit ComputeShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct HlslShaderAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit HlslShaderAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a texture asset.
  struct TextureAsset
  {
  private:
    /// @brief Imported texture.
    std::unique_ptr<ITexture> m_Texture;

  public:
    explicit TextureAsset(std::unique_ptr<ITexture>&& texture)
      : m_Texture(std::move(texture))
    {
    }

    const ITexture&
    GetTexture() const
    {
      return *m_Texture;
    }
  };

  /// @brief Asset containing a Dwarf Engine scene.
  struct SceneAsset
  {
  private:
    nlohmann::json m_SerializedScene;

  public:
    explicit SceneAsset(const std::filesystem::path& path)
      : m_SerializedScene(nlohmann::json::parse(FileHandler::ReadFile(path)))
    {
    }

    const nlohmann::json&
    GetSerializedScene() const
    {
      return m_SerializedScene;
    }
  };

  struct UnknownAsset
  {
  private:
    std::string m_FileContent;

  public:
    explicit UnknownAsset(const std::filesystem::path& path)
      : m_FileContent(FileHandler::ReadFile(path))
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };
}