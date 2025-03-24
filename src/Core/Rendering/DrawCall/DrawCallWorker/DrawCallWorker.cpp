#include "pch.h"

#include "DrawCallWorker.h"

namespace Dwarf
{
  DrawCallWorker::DrawCallWorker(
    std::shared_ptr<IDwarfLogger>      logger,
    std::shared_ptr<ILoadedScene>      loadedScene,
    std::shared_ptr<IDrawCallFactory>  drawCallFactory,
    std::shared_ptr<IDrawCallList>     drawCallList,
    std::shared_ptr<IMeshFactory>      meshFactory,
    std::shared_ptr<IMeshBufferWorker> meshBufferWorker)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mDrawCallFactory(std::move(drawCallFactory))
    , mDrawCallList(std::move(drawCallList))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferWorker(std::move(meshBufferWorker))
  {
    mLogger->LogDebug(Log("DrawCallWorker created!", "DrawCallWorker"));
    mWorkerThread = std::thread([this]() { WorkerThread(); });
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
    std::vector<TempDrawCall>               opagueTemps;
    std::vector<TempDrawCall>               transparentTemps;
    std::vector<std::unique_ptr<IDrawCall>> drawCalls;

    IScene& scene = mLoadedScene->GetScene();

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        Entity e(entity, scene.GetRegistry());

        auto& transform = e.GetComponent<TransformComponent>();
        auto& meshRenderer = e.GetComponent<MeshRendererComponent>();
        auto& model =
          dynamic_cast<ModelAsset&>(meshRenderer.GetModelAsset()->GetAsset());

        for (int i = 0; i < model.Meshes().size(); i++)
        {
          if (model.Meshes().at(i)->GetMaterialIndex() <=
              meshRenderer.MaterialAssets().size())
          {
            if (meshRenderer.MaterialAssets().at(
                  model.Meshes().at(i)->GetMaterialIndex()))
            {
              MaterialAsset& materialAsset = dynamic_cast<MaterialAsset&>(
                meshRenderer.MaterialAssets()
                  .at(model.Meshes().at(i)->GetMaterialIndex())
                  ->GetAsset());

              if (materialAsset.GetMaterial()
                    .GetMaterialProperties()
                    .IsTransparent)
              {
                transparentTemps.emplace_back(*model.Meshes().at(i),
                                              materialAsset.GetMaterial(),
                                              transform);
              }
              else
              {
                opagueTemps.emplace_back(*model.Meshes().at(i),
                                         materialAsset.GetMaterial(),
                                         transform);
              }
            }
          }
        }
      }
    }

    // Sorting the draw calls
    std::ranges::stable_sort(
      opagueTemps,
      [](const TempDrawCall& a, const TempDrawCall& b)
      {
        return std::uintptr_t(std::addressof(a.Material.get())) <
               std::uintptr_t(std::addressof(b.Material.get()));
      });
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
    for (auto& tempDrawCall : opagueTemps)
    {
      if (!currentBatch)
      {
        currentBatch = std::make_unique<Batch>(tempDrawCall.Material.get(),
                                               tempDrawCall.Transform.get());
        currentBatch->Meshes.push_back(tempDrawCall.Mesh.get().Clone());
      }
      else
      {
        if ((std::addressof(currentBatch->Material) !=
             std::addressof(tempDrawCall.Material.get())) ||
            currentBatch->Material.GetMaterialProperties().IsTransparent)
        {
          batches.push_back(std::move(currentBatch));
          currentBatch = std::make_unique<Batch>(tempDrawCall.Material,
                                                 tempDrawCall.Transform);
          currentBatch->Meshes.push_back(tempDrawCall.Mesh.get().Clone());
        }
        else
        {
          currentBatch->Meshes.push_back(tempDrawCall.Mesh.get().Clone());
        }
      }
    }

    if (currentBatch)
    {
      batches.push_back(std::move(currentBatch));
    }

    for (auto& batch : batches)
    {
      std::unique_ptr<IMesh> mergedMesh =
        mMeshFactory->MergeMeshes(batch->Meshes);

      drawCalls.push_back(mDrawCallFactory->Create(
        mergedMesh, batch->Material, batch->Transform));
    }

    for (auto& transparentCall : transparentTemps)
    {
      std::unique_ptr<IMesh> mesh = transparentCall.Mesh.get().Clone();
      drawCalls.push_back(mDrawCallFactory->Create(
        mesh, transparentCall.Material, transparentCall.Transform));
    }

    mDrawCallList->SubmitDrawCalls(std::move(drawCalls));
  }
}