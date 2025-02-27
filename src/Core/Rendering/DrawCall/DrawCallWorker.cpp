#include "DrawCallWorker.h"
#include "IDrawCall.h"
#include "IDrawCallFactory.h"

namespace Dwarf
{
  DrawCallWorker::DrawCallWorker(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IDrawCallFactory> drawCallFactory,
    std::shared_ptr<IDrawCallList>    drawCallList)
    : m_Logger(logger)
    , m_LoadedScene(loadedScene)
    , m_DrawCallFactory(drawCallFactory)
    , m_DrawCallList(drawCallList)
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
  DrawCallSorter(std::unique_ptr<IDrawCall>& a, std::unique_ptr<IDrawCall>& b)
  {
    if (a->GetMaterial().GetMaterialProperties().IsTransparent !=
        b->GetMaterial().GetMaterialProperties().IsTransparent)
    {
      return !a->GetMaterial()
                .GetMaterialProperties()
                .IsTransparent; // false before true
    }
    return &a->GetMaterial() <
           &b->GetMaterial(); // Sort by value if both are not special
  }

  void
  DrawCallWorker::GenerateDrawCalls()
  {
    m_Logger->LogDebug(Log("Generating draw calls", "DrawCallWorker"));
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
        glm::mat4 modelMatrix = transform.GetModelMatrix();

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

              drawCalls.push_back(
                m_DrawCallFactory->Create(*model.Meshes().at(i),
                                          materialAsset.GetMaterial(),
                                          modelMatrix));
            }
          }
        }
      }
    }

    // Sorting the draw calls
    std::sort(drawCalls.begin(), drawCalls.end(), DrawCallSorter);
    m_DrawCallList->SubmitDrawCalls(std::move(drawCalls));
  }
}