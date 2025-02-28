#include "DrawCallWorker.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "IDrawCall.h"
#include "IDrawCallFactory.h"
#include <functional>

namespace Dwarf
{
  DrawCallWorker::DrawCallWorker(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<ILoadedScene>       loadedScene,
    std::shared_ptr<IDrawCallFactory>   drawCallFactory,
    std::shared_ptr<IDrawCallList>      drawCallList,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : m_Logger(logger)
    , m_LoadedScene(loadedScene)
    , m_DrawCallFactory(drawCallFactory)
    , m_DrawCallList(drawCallList)
    , m_MeshFactory(meshFactory)
    , m_MeshBufferFactory(meshBufferFactory)
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
          return;
        }
      }

      // Generating the draw calls
      GenerateDrawCalls();

      m_Invalidate.store(false);
    }
  }

  bool
  DrawCallSorter(TempDrawCall& a, TempDrawCall& b)
  {
    if (a.Material.GetMaterialProperties().IsTransparent !=
        b.Material.GetMaterialProperties().IsTransparent)
    {
      return !a.Material.GetMaterialProperties()
                .IsTransparent; // false before true
    }
    return &a.Material < &b.Material; // Sort by value if both are not special
  }

  void
  DrawCallWorker::GenerateDrawCalls()
  {
    m_Logger->LogDebug(Log("Generating draw calls", "DrawCallWorker"));
    // std::vector<TempDrawCall>               tempDrawCalls;
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

              // tempDrawCalls.push_back({ *model.Meshes().at(i),
              //                           materialAsset.GetMaterial(),
              //                           transform });

              drawCalls.push_back(m_DrawCallFactory->Create(
                m_MeshBufferFactory->Create(*model.Meshes().at(i)),
                materialAsset.GetMaterial(),
                transform));
              // drawCalls.push_back(m_DrawCallFactory->Create(
              //   *model.Meshes().at(i), materialAsset.GetMaterial(),
              //   transform));
            }
          }
        }
      }
    }

    // Sorting the draw calls
    // std::sort(tempDrawCalls.begin(), tempDrawCalls.end(), DrawCallSorter);

    // TempDrawCall& first = tempDrawCalls[0];

    // IMaterial&                                 currentMaterial =
    // first.Material; std::vector<std::reference_wrapper<IMesh>> currentBatch;
    //  Batch per Material and upload geometry
    // for (auto& tempDrawCall : tempDrawCalls)
    //{
    // drawCalls.push_back(m_DrawCallFactory->Create(
    //   std::move(m_MeshBufferFactory->Create(tempDrawCall.Mesh)),
    //   tempDrawCall.Material,
    //   tempDrawCall.Transform));
    // Merge when still on the same material
    /*if ((&tempDrawCall.Material == &currentMaterial) &&
        !currentMaterial.GetMaterialProperties().IsTransparent)
    {
      currentBatch.push_back(tempDrawCall.Mesh);
    }
    else
    {
      std::unique_ptr<IMesh> mergedMesh =
        std::move(m_MeshFactory->MergeMeshes(currentBatch));

      std::unique_ptr<IMeshBuffer> meshBuffer =
        m_MeshBufferFactory->Create(*mergedMesh);

      drawCalls.push_back(m_DrawCallFactory->Create(std::move(meshBuffer),
                                                    tempDrawCall.Material,
                                                    tempDrawCall.Transform));

      currentMaterial = tempDrawCall.Material;
      currentBatch.clear();
      currentBatch.push_back(tempDrawCall.Mesh);
    }*/
    //}

    m_DrawCallList->SubmitDrawCalls(std::move(drawCalls));
  }
}