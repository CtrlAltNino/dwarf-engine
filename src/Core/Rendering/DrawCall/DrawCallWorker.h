#pragma once

#include "Core/Rendering/DrawCall/IDrawCallList.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "IDrawCallFactory.h"
#include "IDrawCallWorker.h"
#include "Logging/IDwarfLogger.h"
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace Dwarf
{
  struct TempDrawCall
  {
    std::reference_wrapper<IMesh>              Mesh;
    std::reference_wrapper<IMaterial>          Material;
    std::reference_wrapper<TransformComponent> Transform;

    TempDrawCall(IMesh&              mesh,
                 IMaterial&          material,
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

    TempDrawCall&
    operator=(TempDrawCall&& other) noexcept
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
    IMaterial&                          Material;
    TransformComponent&                 Transform;

    Batch(IMaterial& material, TransformComponent& transform)
      : Material(material)
      , Transform(transform)
    {
    }
  };

  class DrawCallWorker : public IDrawCallWorker
  {
  private:
    std::thread                        m_WorkerThread;
    std::shared_ptr<IDwarfLogger>      m_Logger;
    std::shared_ptr<ILoadedScene>      m_LoadedScene;
    std::shared_ptr<IDrawCallFactory>  m_DrawCallFactory;
    std::shared_ptr<IDrawCallList>     m_DrawCallList;
    std::shared_ptr<IMeshFactory>      m_MeshFactory;
    std::shared_ptr<IMeshBufferWorker> m_MeshBufferWorker;
    std::condition_variable            m_Condition;
    std::atomic<bool>                  m_StopWorker = false;
    std::atomic<bool>                  m_Invalidate = false;
    std::mutex                         m_ThreadMutex;

  public:
    DrawCallWorker(std::shared_ptr<IDwarfLogger>      logger,
                   std::shared_ptr<ILoadedScene>      loadedScene,
                   std::shared_ptr<IDrawCallFactory>  drawCallFactory,
                   std::shared_ptr<IDrawCallList>     drawCallList,
                   std::shared_ptr<IMeshFactory>      meshFactory,
                   std::shared_ptr<IMeshBufferWorker> meshBufferWorker);

    ~DrawCallWorker() override;

    void
    Invalidate() override;

    void
    WorkerThread();

    void
    GenerateDrawCalls();
  };
}