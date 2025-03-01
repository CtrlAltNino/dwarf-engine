#include "DrawCallWorker.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "IDrawCall.h"
#include "IDrawCallFactory.h"
#include <functional>
#include <algorithm>
#include <memory>

namespace Dwarf
{
  DrawCallWorker::DrawCallWorker(
    std::shared_ptr<IDwarfLogger>      logger,
    std::shared_ptr<ILoadedScene>      loadedScene,
    std::shared_ptr<IDrawCallFactory>  drawCallFactory,
    std::shared_ptr<IDrawCallList>     drawCallList,
    std::shared_ptr<IMeshFactory>      meshFactory,
    std::shared_ptr<IMeshBufferWorker> meshBufferWorker)
    : m_Logger(logger)
    , m_LoadedScene(loadedScene)
    , m_DrawCallFactory(drawCallFactory)
    , m_DrawCallList(drawCallList)
    , m_MeshFactory(meshFactory)
    , m_MeshBufferWorker(meshBufferWorker)
  {
    m_Logger->LogDebug(Log("DrawCallWorker created!", "DrawCallWorker"));
    m_WorkerThread = std::thread([this]() { WorkerThread(); });
  }

  DrawCallWorker::~DrawCallWorker()
  {
    m_Logger->LogDebug(Log("Destroying DrawCallWorker", "DrawCallWorker"));
    {
      std::lock_guard<std::mutex> lock(m_ThreadMutex);
      m_StopWorker.store(true);
    }
    m_Condition.notify_one();

    if (m_WorkerThread.joinable())
    {
      m_Logger->LogDebug(Log("Joining Worker Thread", "DrawCallWorker"));
      m_WorkerThread.join();
      m_Logger->LogDebug(Log("Worker Thread joined", "DrawCallWorker"));
    }
  }

  void
  DrawCallWorker::Invalidate()
  {
    {
      std::lock_guard<std::mutex> lock(m_ThreadMutex);
      m_Invalidate.store(true);
    }
    m_Condition.notify_one();
  }

  void
  DrawCallWorker::WorkerThread()
  {
    m_Logger->LogDebug(Log("DrawCallWorker started", "DrawCallWorker"));
    while (!m_StopWorker.load())
    {
      m_Logger->LogDebug(Log("Ready to generate draw calls", "DrawCallWorker"));
      // Waiting until the current draw calls are being invalidated or the
      // worker should be stopped
      {
        std::unique_lock<std::mutex> lock(m_ThreadMutex);
        m_Condition.wait(
          lock, [this] { return m_Invalidate.load() || m_StopWorker.load(); });

        m_Logger->LogDebug(Log("Woken up", "DrawCallWorker"));
        if (m_StopWorker.load())
        {
          m_Logger->LogDebug(Log("Stop flag set", "DrawCallWorker"));
        }
      }

      if (!m_StopWorker.load())
      {
        // Generating the draw calls
        GenerateDrawCalls();

        m_Invalidate.store(false);
      }
    }
  }

  void
  DrawCallWorker::GenerateDrawCalls()
  {
    m_Logger->LogDebug(Log("Generating draw calls", "DrawCallWorker"));
    std::vector<TempDrawCall>               opagueTemps;
    std::vector<TempDrawCall>               transparentTemps;
    std::vector<std::unique_ptr<IDrawCall>> drawCalls;

    IScene& scene = m_LoadedScene->GetScene();

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        Entity e(entity, scene.GetRegistry());

        TransformComponent&    transform = e.GetComponent<TransformComponent>();
        MeshRendererComponent& meshRenderer =
          e.GetComponent<MeshRendererComponent>();
        ModelAsset& model =
          (ModelAsset&)meshRenderer.GetModelAsset()->GetAsset();

        for (int i = 0; i < model.Meshes().size(); i++)
        {
          if (model.Meshes().at(i)->GetMaterialIndex() <=
              meshRenderer.MaterialAssets().size())
          {
            if (meshRenderer.MaterialAssets().at(
                  model.Meshes().at(i)->GetMaterialIndex()))
            {
              MaterialAsset& materialAsset =
                (MaterialAsset&)meshRenderer.MaterialAssets()
                  .at(model.Meshes().at(i)->GetMaterialIndex())
                  ->GetAsset();

              if (materialAsset.GetMaterial()
                    .GetMaterialProperties()
                    .IsTransparent)
              {
                transparentTemps.push_back({ *model.Meshes().at(i),
                                             materialAsset.GetMaterial(),
                                             transform });
              }
              else
              {
                opagueTemps.push_back({ *model.Meshes().at(i),
                                        materialAsset.GetMaterial(),
                                        transform });
              }

              // drawCalls.push_back(m_DrawCallFactory->Create(
              //   model.Meshes().at(i), materialAsset.GetMaterial(),
              //   transform));
            }
          }
        }
      }
    }

    // Sorting the draw calls
    std::stable_sort(opagueTemps.begin(),
                     opagueTemps.end(),
                     [](const TempDrawCall& a, const TempDrawCall& b)
                     {
                       return std::uintptr_t(std::addressof(a.Material.get())) <
                              std::uintptr_t(std::addressof(b.Material.get()));
                     });
    std::stable_sort(transparentTemps.begin(),
                     transparentTemps.end(),
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
      std::cout << &tempDrawCall.Material << std::endl;
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
        m_MeshFactory->MergeMeshes(batch->Meshes);

      drawCalls.push_back(m_DrawCallFactory->Create(
        mergedMesh, batch->Material, batch->Transform));
    }

    for (auto& transparentCall : transparentTemps)
    {
      std::unique_ptr<IMesh> mesh = transparentCall.Mesh.get().Clone();
      drawCalls.push_back(m_DrawCallFactory->Create(
        mesh, transparentCall.Material, transparentCall.Transform));
    }

    m_DrawCallList->SubmitDrawCalls(std::move(drawCalls));
  }
}