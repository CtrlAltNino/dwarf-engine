#pragma once

#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "Core/Rendering/DrawCall/IDrawCallFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "IDrawCallWorker.h"
#include "Logging/IDwarfLogger.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace Dwarf
{
  struct TempDrawCall
  {
    std::reference_wrapper<IMesh>              Mesh;
    std::reference_wrapper<MaterialAsset>      Material;
    std::reference_wrapper<TransformComponent> Transform;

    TempDrawCall(IMesh&              mesh,
                 MaterialAsset&      material,
                 TransformComponent& transform)
      : Mesh(mesh)
      , Material(material)
      , Transform(transform)
    {
    }

    // Move constructor
    TempDrawCall(TempDrawCall&& other) noexcept
      : Mesh(other.Mesh)
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
    std::vector<std::unique_ptr<IMesh>> Meshes;
    MaterialAsset&                      Material;
    TransformComponent&                 Transform;

    Batch(MaterialAsset& material, TransformComponent& transform)
      : Material(material)
      , Transform(transform)
    {
    }
  };

  class DrawCallWorker : public IDrawCallWorker
  {
  private:
    std::thread                        mWorkerThread;
    std::shared_ptr<IDwarfLogger>      mLogger;
    std::shared_ptr<ILoadedScene>      mLoadedScene;
    std::shared_ptr<IDrawCallFactory>  mDrawCallFactory;
    std::unique_ptr<IDrawCallList>&    mDrawCallList;
    std::shared_ptr<IMeshFactory>      mMeshFactory;
    std::shared_ptr<IMeshBufferWorker> mMeshBufferWorker;
    std::condition_variable            mCondition;
    std::atomic<bool>                  mStopWorker = false;
    std::atomic<bool>                  mInvalidate = false;
    std::mutex                         mThreadMutex;

  public:
    DrawCallWorker(std::shared_ptr<IDwarfLogger>      logger,
                   std::shared_ptr<ILoadedScene>      loadedScene,
                   std::shared_ptr<IDrawCallFactory>  drawCallFactory,
                   std::unique_ptr<IDrawCallList>&    drawCallList,
                   std::shared_ptr<IMeshFactory>      meshFactory,
                   std::shared_ptr<IMeshBufferWorker> meshBufferWorker);

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
  };
}