#pragma once

#include "Core/Rendering/DrawCall/IDrawCallList.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
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
  class DrawCallWorker : public IDrawCallWorker
  {
  private:
    std::thread                       m_WorkerThread;
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<ILoadedScene>     m_LoadedScene;
    std::shared_ptr<IDrawCallFactory> m_DrawCallFactory;
    std::shared_ptr<IDrawCallList>    m_DrawCallList;
    std::condition_variable           m_Condition;
    std::atomic<bool>                 m_StopWorker = false;
    std::atomic<bool>                 m_Invalidate = false;
    std::mutex                        m_ThreadMutex;

  public:
    DrawCallWorker(std::shared_ptr<IDwarfLogger>     logger,
                   std::shared_ptr<ILoadedScene>     loadedScene,
                   std::shared_ptr<IDrawCallFactory> drawCallFactory,
                   std::shared_ptr<IDrawCallList>    drawCallList);

    ~DrawCallWorker() override;

    void
    Invalidate() override;

    void
    WorkerThread();

    void
    GenerateDrawCalls();
  };
}