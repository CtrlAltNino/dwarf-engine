#pragma once

#include "Core/Rendering/Texture/ITexture.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Material/IMaterial.h"
#include <optional>

namespace Dwarf
{
  // Abstract struct for asset components
  struct IAssetComponent
  {
    virtual ~IAssetComponent() = default;
  };

  /// @brief Component containing a model asset.
  struct ModelAsset : public IAssetComponent
  {
  private:
    /// @brief Vector of submeshes.
    std::vector<std::unique_ptr<IMesh>> m_Meshes = {};

  public:
    ModelAsset() {}
    ModelAsset(std::vector<std::unique_ptr<IMesh>> meshes)
      : m_Meshes(std::move(meshes))
    {
      for (auto& mesh : meshes)
      {
        mesh->SetupMesh();
      }
    }

    // copy constructors using the clone function
    ModelAsset(const ModelAsset& other)
    {
      for (const auto& mesh : other.m_Meshes)
      {
        m_Meshes.push_back(mesh->Clone());
      }
    }

    ModelAsset&
    operator=(const ModelAsset& other)
    {
      if (this == &other)
      {
        return *this;
      }

      m_Meshes.clear();
      for (const auto& mesh : other.m_Meshes)
      {
        m_Meshes.push_back(mesh->Clone());
      }

      return *this;
    }

    std::vector<std::unique_ptr<IMesh>>&
    Meshes()
    {
      return m_Meshes;
    }

    const std::vector<std::unique_ptr<IMesh>>&
    Meshes() const
    {
      return m_Meshes;
    }
  };

  /// @brief Component containing a material asset.
  struct MaterialAsset : public IAssetComponent
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
  struct VertexShaderAsset : public IAssetComponent
  {
  private:
    /// @brief The content of the file.
    std::string m_FileContent;

  public:
    explicit VertexShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a fragment shader asset.
  struct FragmentShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit FragmentShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a geometry shader asset.
  struct GeometryShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit GeometryShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a tesselation control shader asset.
  struct TessellationControlShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit TessellationControlShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a tesselation evaluation shader asset.
  struct TessellationEvaluationShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit TessellationEvaluationShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct ComputeShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit ComputeShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct HlslShaderAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit HlslShaderAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };

  /// @brief Component containing a texture asset.
  struct TextureAsset : public IAssetComponent
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
  struct SceneAsset : public IAssetComponent
  {
  private:
    nlohmann::json m_SerializedScene;

  public:
    explicit SceneAsset(const nlohmann::json& serializedScene)
      : m_SerializedScene(serializedScene)
    {
    }

    const nlohmann::json&
    GetSerializedScene() const
    {
      return m_SerializedScene;
    }
  };

  struct UnknownAsset : public IAssetComponent
  {
  private:
    std::string m_FileContent;

  public:
    explicit UnknownAsset(const std::string& fileContent)
      : m_FileContent(fileContent)
    {
    }

    const std::string&
    GetFileContent() const
    {
      return m_FileContent;
    }
  };
}