#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Rendering/DrawCall/IDrawCallFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IDrawCallWorkerFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

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
    std::shared_ptr<IAssetDatabase>    mAssetDatabase;

  public:
    DrawCallWorkerFactory(std::shared_ptr<IDwarfLogger>      logger,
                          std::shared_ptr<ILoadedScene>      loadedScene,
                          std::shared_ptr<IDrawCallFactory>  drawCallFactory,
                          std::shared_ptr<IMeshFactory>      meshFactory,
                          std::shared_ptr<IMeshBufferWorker> meshBufferWorker,
                          std::shared_ptr<IAssetDatabase>    assetDatabase);
    ~DrawCallWorkerFactory() override;

    [[nodiscard]] auto
    Create(std::unique_ptr<IDrawCallList>& drawCallList) const
      -> std::unique_ptr<IDrawCallWorker> override;
  };
}