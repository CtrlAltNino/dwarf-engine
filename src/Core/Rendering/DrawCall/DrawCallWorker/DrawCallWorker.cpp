#include "pch.hpp"

#include "DrawCallWorker.hpp"

namespace Dwarf
{
  DrawCallWorker::DrawCallWorker(
    std::shared_ptr<IDwarfLogger>      logger,
    std::shared_ptr<ILoadedScene>      loadedScene,
    std::shared_ptr<IDrawCallFactory>  drawCallFactory,
    std::unique_ptr<IDrawCallList>&    drawCallList,
    std::shared_ptr<IMeshFactory>      meshFactory,
    std::shared_ptr<IMeshBufferWorker> meshBufferWorker,
    std::shared_ptr<IAssetDatabase>    assetDatabase)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mDrawCallFactory(std::move(drawCallFactory))
    , mDrawCallList(drawCallList)
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferWorker(std::move(meshBufferWorker))
    , mAssetDatabase(std::move(assetDatabase))
  {
    mLogger->LogDebug(Log("DrawCallWorker created", "DrawCallWorker"));
    mWorkerThread = std::thread([this]() { WorkerThread(); });
    mLoadedScene->RegisterLoadedSceneObserver(this);
    mAssetDatabase->RegisterAssetDatabaseObserver(this);
  }

  DrawCallWorker::~DrawCallWorker()
  {
    mLogger->LogDebug(Log("Destroying DrawCallWorker", "DrawCallWorker"));
    {
      std::lock_guard<std::mutex> lock(mThreadMutex);
      mStopWorker.store(true);
    }
    mCondition.notify_one();

    if (mWorkerThread.joinable())
    {
      mLogger->LogDebug(Log("Joining Worker Thread", "DrawCallWorker"));
      mWorkerThread.join();
      mLogger->LogDebug(Log("Worker Thread joined", "DrawCallWorker"));
    }

    mLoadedScene->UnregisterLoadedSceneObserver(this);
    mLogger->LogDebug(Log("DrawCallWorker destroyed", "DrawCallWorker"));
  }

  void
  DrawCallWorker::Invalidate()
  {
    {
      std::lock_guard<std::mutex> lock(mThreadMutex);
      mInvalidate.store(true);
    }
    mCondition.notify_one();
  }

  void
  DrawCallWorker::WorkerThread()
  {
    mLogger->LogDebug(Log("DrawCallWorker started", "DrawCallWorker"));
    while (!mStopWorker.load())
    {
      mLogger->LogDebug(Log("Ready to generate draw calls", "DrawCallWorker"));
      // Waiting until the current draw calls are being invalidated or the
      // worker should be stopped
      {
        std::unique_lock<std::mutex> lock(mThreadMutex);
        mCondition.wait(
          lock, [this] { return mInvalidate.load() || mStopWorker.load(); });

        mLogger->LogDebug(Log("Woken up", "DrawCallWorker"));
        if (mStopWorker.load())
        {
          mLogger->LogDebug(Log("Stop flag set", "DrawCallWorker"));
        }
      }

      if (!mStopWorker.load())
      {
        // Generating the draw calls
        GenerateDrawCalls();

        mInvalidate.store(false);
      }
    }
  }

  void
  DrawCallWorker::GenerateDrawCalls()
  {
    mLogger->LogDebug(Log("Generating draw calls", "DrawCallWorker"));
    std::vector<TempDrawCall>               batchedTemps;
    std::vector<TempDrawCall>               transparentTemps;
    std::vector<std::unique_ptr<IDrawCall>> drawCalls;

    if (!mLoadedScene->HasLoadedScene())
    {
      mLogger->LogDebug(
        Log("No scene loaded to generate draw calls from", "DrawCallWorker"));
      return;
    }

    IScene& scene = mLoadedScene->GetScene();

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entityHandle, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        auto& model =
          dynamic_cast<ModelAsset&>(meshRenderer.GetModelAsset()->GetAsset());

        for (const auto& mesh : model.Meshes())
        {
          if (meshRenderer.MaterialAssets().contains(
                mesh->GetMaterialIndex()) &&
              meshRenderer.MaterialAssets()[mesh->GetMaterialIndex()])
          {
            MaterialAsset& materialAsset =
              dynamic_cast<MaterialAsset&>(meshRenderer.MaterialAssets()
                                             .at(mesh->GetMaterialIndex())
                                             ->GetAsset());

            if (materialAsset.GetMaterial()
                  .GetMaterialProperties()
                  .IsTransparent)
            {
              transparentTemps.emplace_back(*mesh, materialAsset, transform);
            }
            else
            {
              batchedTemps.emplace_back(*mesh, materialAsset, transform);
            }
          }
        }
      }
    }

    // Sorting the draw calls
    std::ranges::stable_sort(
      batchedTemps,
      [](const TempDrawCall& a, const TempDrawCall& b)
      {
        return std::uintptr_t(std::addressof(a.Material.get())) <
               std::uintptr_t(std::addressof(b.Material.get()));
      });

    // TODO: Add transparency ordering (e.g. back to front)
    std::ranges::stable_sort(
      transparentTemps,
      [](const TempDrawCall& a, const TempDrawCall& b)
      {
        return std::uintptr_t(std::addressof(a.Material.get())) <
               std::uintptr_t(std::addressof(b.Material.get()));
      });

    std::vector<std::unique_ptr<Batch>> batches;
    std::unique_ptr<Batch>              currentBatch;

    // Batch per Material and upload geometry
    for (auto& currentTempDrawCall : batchedTemps)
    {
      // No current batch, creating new and adding the current draw call
      if (!currentBatch)
      {
        // Create new batch with material
        currentBatch = std::make_unique<Batch>(
          currentTempDrawCall.Material.get(), currentTempDrawCall.Transform);

        // Add the geometry of the current temporary draw call
        currentBatch->Meshes.emplace_back(
          currentTempDrawCall.Mesh.get().Clone());
      }
      else
      {
        // If there is a current batch and we encounter a different material or
        // a different entity
        if ((std::addressof(currentBatch->Material) !=
             std::addressof(currentTempDrawCall.Material.get())) ||
            (std::addressof(currentBatch->Transform) !=
             std::addressof(currentTempDrawCall.Transform.get())))
        {
          // Push the current batch
          batches.push_back(std::move(currentBatch));

          // Create new batch with material
          currentBatch = std::make_unique<Batch>(
            currentTempDrawCall.Material.get(), currentTempDrawCall.Transform);

          // Add the geometry of the current temporary draw call
          currentBatch->Meshes.emplace_back(
            currentTempDrawCall.Mesh.get().Clone());
        }
        // We can still add to the current batch
        else
        {
          currentBatch->Meshes.push_back(
            currentTempDrawCall.Mesh.get().Clone());
        }
      }
    }

    // If we got an unpushed batch dangling
    if (currentBatch)
    {
      batches.push_back(std::move(currentBatch));
    }

    // Generate draw calls from batches
    for (auto& batch : batches)
    {
      std::unique_ptr<IMesh> mergedMesh =
        mMeshFactory->MergeMeshes(batch->Meshes);

      drawCalls.push_back(mDrawCallFactory->Create(
        mergedMesh, batch->Material, batch->Transform));
    }

    // Generate draw calls for transparent materials
    for (auto& transparentCall : transparentTemps)
    {
      std::unique_ptr<IMesh> mesh = transparentCall.Mesh.get().Clone();
      drawCalls.push_back(mDrawCallFactory->Create(
        mesh, transparentCall.Material, transparentCall.Transform));
    }

    mDrawCallList->SubmitDrawCalls(std::move(drawCalls));
  }

  void
  DrawCallWorker::OnSceneLoad()
  {
    mDrawCallList->Clear();
    this->Invalidate();
  }

  void
  DrawCallWorker::OnSceneUnload()
  {
    mDrawCallList->Clear();
  }

  void
  DrawCallWorker::OnReimportAll()
  {
    mDrawCallList->Clear();
    this->Invalidate();
  }
}