#pragma once

#include "Core/Asset/Database/IAssetComponent.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Texture/ITexture.h"

namespace Dwarf
{

  /// @brief Component containing a model asset.
  struct ModelAsset : public IAssetComponent
  {
  private:
    /// @brief Vector of submeshes.
    std::vector<std::unique_ptr<IMesh>> mMeshes;

  public:
    ModelAsset() = default;

    ModelAsset(std::vector<std::unique_ptr<IMesh>>&& meshes)
      : mMeshes(std::move(meshes))
    {
    }

    // copy constructors using the clone function
    ModelAsset(const ModelAsset& other)
    {
      for (const auto& mesh : other.mMeshes)
      {
        mMeshes.push_back(mesh->Clone());
      }
    }

    auto
    operator=(const ModelAsset& other) -> ModelAsset&
    {
      if (this == &other)
      {
        return *this;
      }

      mMeshes.clear();
      for (const auto& mesh : other.mMeshes)
      {
        mMeshes.push_back(mesh->Clone());
      }

      return *this;
    }

    auto
    Meshes() -> std::vector<std::unique_ptr<IMesh>>&
    {
      return mMeshes;
    }

    auto
    Meshes() const -> const std::vector<std::unique_ptr<IMesh>>&
    {
      return mMeshes;
    }
  };

  /// @brief Component containing a material asset.
  struct MaterialAsset : public IAssetComponent
  {
  private:
    /// @brief Imported material.
    std::unique_ptr<IMaterial> mMaterial;

  public:
    MaterialAsset(std::unique_ptr<IMaterial>&& material)
      : mMaterial(std::move(material))
    {
    }

    auto
    GetMaterial() -> IMaterial&
    {
      return *mMaterial;
    }

    [[nodiscard]] auto
    GetMaterial() const -> const IMaterial&
    {
      return *mMaterial;
    }
  };

  /// @brief Component containing a vertex shader asset.
  struct VertexShaderAsset : public IAssetComponent
  {
  private:
    /// @brief The content of the file.
    std::string mFileContent;

  public:
    explicit VertexShaderAsset(const std::string& fileContent)
      : mFileContent(fileContent)
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a fragment shader asset.
  struct FragmentShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit FragmentShaderAsset(const std::string& fileContent)
      : mFileContent(fileContent)
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a geometry shader asset.
  struct GeometryShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit GeometryShaderAsset(const std::string& fileContent)
      : mFileContent(fileContent)
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a tesselation control shader asset.
  struct TessellationControlShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit TessellationControlShaderAsset(const std::string& fileContent)
      : mFileContent(fileContent)
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a tesselation evaluation shader asset.
  struct TessellationEvaluationShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit TessellationEvaluationShaderAsset(const std::string& fileContent)
      : mFileContent(fileContent)
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct ComputeShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit ComputeShaderAsset(std::string fileContent)
      : mFileContent(std::move(fileContent))
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a compute shader asset.
  struct HlslShaderAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit HlslShaderAsset(std::string fileContent)
      : mFileContent(std::move(fileContent))
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };

  /// @brief Component containing a texture asset.
  struct TextureAsset : public IAssetComponent
  {
  private:
    /// @brief Imported texture.
    std::unique_ptr<ITexture>        mTexture;
    std::reference_wrapper<ITexture> mPlaceholder;
    bool                             mIsCurrentlyLoading = false;

  public:
    explicit TextureAsset(std::unique_ptr<ITexture>&& texture,
                          ITexture&                   placeholder)
      : mTexture(std::move(texture))
      , mPlaceholder(placeholder)
    {
    }

    /**
     * @brief Return whether the texture asset has been loaded into the GPU
     *
     * @return true If the texture is present on the GPU
     * @return false If the texture is not present on the GPU
     */
    auto
    IsLoaded() const -> bool
    {
      return mTexture != nullptr;
    }

    /**
     * @brief Returns a reference to the texture, or if it is not loaded yet, to
     * the placeholder
     *
     * @return Reference to the texture
     */
    auto
    GetTexture() const -> const ITexture&
    {
      if (!mTexture)
      {
        return mPlaceholder;
      }

      return *mTexture;
    }

    /**
     * @brief Sets the texture
     *
     * @param texture The texture that should be moved into the asset component
     */
    void
    SetTexture(std::unique_ptr<ITexture>&& texture)
    {
      mTexture = std::move(texture);
    }
  };

  /// @brief Asset containing a Dwarf Engine scene.
  struct SceneAsset : public IAssetComponent
  {
  private:
    nlohmann::json mSerializedScene;

  public:
    explicit SceneAsset(nlohmann::json serializedScene)
      : mSerializedScene(std::move(serializedScene))
    {
    }

    auto
    GetSerializedScene() const -> const nlohmann::json&
    {
      return mSerializedScene;
    }
  };

  struct UnknownAsset : public IAssetComponent
  {
  private:
    std::string mFileContent;

  public:
    explicit UnknownAsset(std::string fileContent)
      : mFileContent(std::move(fileContent))
    {
    }

    auto
    GetFileContent() const -> const std::string&
    {
      return mFileContent;
    }
  };
}