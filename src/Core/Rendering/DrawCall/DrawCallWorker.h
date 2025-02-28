#pragma once

#include "Core/Rendering/DrawCall/IDrawCallList.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "IDrawCallFactory.h"
#include "IDrawCallWorker.h"
#include "Logging/IDwarfLogger.h"
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace Dwarf
{
  struct TempDrawCall
  {
    IMesh&              Mesh;
    IMaterial&          Material;
    TransformComponent& Transform;

    TempDrawCall(IMesh&              mesh,
                 IMaterial&          material,
                 TransformComponent& transform)
      : Mesh(mesh)
      , Material(material)
      , Transform(transform)
    {
    }

    TempDrawCall&
    operator=(const TempDrawCall& other)
    {
      if (this == &other) return *this; // Self-assignment check

      // Copy new resource
      Mesh = other.Mesh;
      Material = other.Material;
      Transform = other.Transform;

      return *this;
    }
  };

  class DrawCallWorker : public IDrawCallWorker
  {
  private:
    std::thread                         m_WorkerThread;
    std::shared_ptr<IDwarfLogger>       m_Logger;
    std::shared_ptr<ILoadedScene>       m_LoadedScene;
    std::shared_ptr<IDrawCallFactory>   m_DrawCallFactory;
    std::shared_ptr<IDrawCallList>      m_DrawCallList;
    std::shared_ptr<IMeshFactory>       m_MeshFactory;
    std::shared_ptr<IMeshBufferFactory> m_MeshBufferFactory;
    std::condition_variable             m_Condition;
    std::atomic<bool>                   m_StopWorker = false;
    std::atomic<bool>                   m_Invalidate = false;
    std::mutex                          m_ThreadMutex;

  public:
    DrawCallWorker(std::shared_ptr<IDwarfLogger>       logger,
                   std::shared_ptr<ILoadedScene>       loadedScene,
                   std::shared_ptr<IDrawCallFactory>   drawCallFactory,
                   std::shared_ptr<IDrawCallList>      drawCallList,
                   std::shared_ptr<IMeshFactory>       meshFactory,
                   std::shared_ptr<IMeshBufferFactory> meshBufferFactory);

    ~DrawCallWorker() override;

    void
    Invalidate() override;

    void
    WorkerThread();

    void
    GenerateDrawCalls();
  };
}