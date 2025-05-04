#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Database/IAssetDatabaseObserver.hpp"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.hpp"
#include "Core/Rendering/DrawCall/IDrawCallFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/MeshBufferRequestList/IMeshBufferRequestList.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IDrawCallWorker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

namespace Dwarf
{
  struct TempDrawCall
  {
    std::shared_ptr<IMesh>                Mesh;
    std::reference_wrapper<MaterialAsset> Material;
    TransformComponentHandle              Transform;

    TempDrawCall(std::shared_ptr<IMesh>   mesh,
                 MaterialAsset&           material,
                 TransformComponentHandle transform)
      : Mesh(std::move(mesh))
      , Material(material)
      , Transform(transform)
    {
    }

    // Move constructor
    TempDrawCall(TempDrawCall&& other) noexcept
      : Mesh(std::move(other.Mesh))
      , Material(other.Material)
      , Transform(other.Transform)
    {
    } // The reference member can't be moved, so just initialize it

    auto
    operator=(TempDrawCall&& other) noexcept -> TempDrawCall&
    {
      if (this != &other)
      {
        // No need to move `ref` because it's a reference, not an object that
        // can be moved
        Mesh = other.Mesh; // Move the non-reference members
        Material = other.Material;
        Transform = other.Transform;
      }
      return *this;
    }
  };

  struct Batch
  {
    std::vector<std::shared_ptr<IMesh>> Meshes;
    MaterialAsset&                      Material;
    TransformComponentHandle            Transform;

    Batch(MaterialAsset& material, TransformComponentHandle transform)
      : Material(material)
      , Transform(transform)
    {
    }
  };

  class DrawCallWorker
    : public IDrawCallWorker
    , public ILoadedSceneObserver
    , public IAssetDatabaseObserver
  {
  private:
    std::thread                             mWorkerThread;
    std::shared_ptr<IDwarfLogger>           mLogger;
    std::shared_ptr<ILoadedScene>           mLoadedScene;
    std::shared_ptr<IDrawCallFactory>       mDrawCallFactory;
    std::unique_ptr<IDrawCallList>&         mDrawCallList;
    std::shared_ptr<IMeshFactory>           mMeshFactory;
    std::shared_ptr<IMeshBufferRequestList> mMeshBufferRequestList;
    std::shared_ptr<IAssetDatabase>         mAssetDatabase;
    std::condition_variable                 mCondition;
    std::atomic<bool>                       mStopWorker = false;
    std::atomic<bool>                       mInvalidate = false;
    std::mutex                              mThreadMutex;

  public:
    DrawCallWorker(
      std::shared_ptr<IDwarfLogger>           logger,
      std::shared_ptr<ILoadedScene>           loadedScene,
      std::shared_ptr<IDrawCallFactory>       drawCallFactory,
      std::unique_ptr<IDrawCallList>&         drawCallList,
      std::shared_ptr<IMeshFactory>           meshFactory,
      std::shared_ptr<IMeshBufferRequestList> MeshBufferRequestList,
      std::shared_ptr<IAssetDatabase>         assetDatabase);

    ~DrawCallWorker() override;

    /**
     * @brief Signals to rebuild the draw calls
     *
     */
    void
    Invalidate() override;

    /**
     * @brief Worker function that runs in a separate thread to build draw calls
     *
     */
    void
    WorkerThread();

    /**
     * @brief Generates draw calls based on the loaded scene
     *
     */
    void
    GenerateDrawCalls();

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;

    void
    OnReimportAll() override;

    void
    OnReimportAsset(const std::filesystem::path& assetPath,
                    ASSET_TYPE                   assetType,
                    const UUID&                  uid) override;

    void
    OnImportAsset(const std::filesystem::path& assetPath,
                  ASSET_TYPE                   assetType,
                  const UUID&                  uid) override;

    void
    OnAssetDatabaseClear() override;

    void
    OnRemoveAsset(const std::filesystem::path& path) override;

    void
    OnRename(const std::filesystem::path& oldPath,
             const std::filesystem::path& newPath) override;

    void
    OnMeshRendererComponentChange(entt::registry& registry,
                                  entt::entity    entity);
  };
}