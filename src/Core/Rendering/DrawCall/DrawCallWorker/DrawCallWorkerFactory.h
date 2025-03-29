#pragma once

#include "Core/Rendering/DrawCall/IDrawCallFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "IDrawCallWorkerFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class DrawCallWorkerFactory : public IDrawCallWorkerFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>      mLogger;
    std::shared_ptr<ILoadedScene>      mLoadedScene;
    std::shared_ptr<IDrawCallFactory>  mDrawCallFactory;
    std::shared_ptr<IMeshFactory>      mMeshFactory;
    std::shared_ptr<IMeshBufferWorker> mMeshBufferWorker;

  public:
    DrawCallWorkerFactory(std::shared_ptr<IDwarfLogger>      logger,
                          std::shared_ptr<ILoadedScene>      loadedScene,
                          std::shared_ptr<IDrawCallFactory>  drawCallFactory,
                          std::shared_ptr<IMeshFactory>      meshFactory,
                          std::shared_ptr<IMeshBufferWorker> meshBufferWorker);
    ~DrawCallWorkerFactory() override;

    [[nodiscard]] auto
    Create(std::unique_ptr<IDrawCallList>& drawCallList) const
      -> std::unique_ptr<IDrawCallWorker> override;
  };
}