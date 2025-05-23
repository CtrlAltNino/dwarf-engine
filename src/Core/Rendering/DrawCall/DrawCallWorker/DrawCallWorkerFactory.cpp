#include "pch.hpp"

#include "DrawCallWorker.hpp"
#include "DrawCallWorkerFactory.hpp"

namespace Dwarf
{
  DrawCallWorkerFactory::DrawCallWorkerFactory(
    std::shared_ptr<IDwarfLogger>           logger,
    std::shared_ptr<ILoadedScene>           loadedScene,
    std::shared_ptr<IDrawCallFactory>       drawCallFactory,
    std::shared_ptr<IMeshFactory>           meshFactory,
    std::shared_ptr<IMeshBufferRequestList> MeshBufferRequestList,
    std::shared_ptr<IAssetDatabase>         assetDatabase)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mDrawCallFactory(std::move(drawCallFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferRequestList(std::move(MeshBufferRequestList))
    , mAssetDatabase(std::move(assetDatabase))
  {
    mLogger->LogDebug(
      Log("DrawCallWorkerFactory created", "DrawCallWorkerFactory"));
  }

  DrawCallWorkerFactory::~DrawCallWorkerFactory()
  {
    mLogger->LogDebug(
      Log("DrawCallWorkerFactory destroyed", "DrawCallWorkerFactory"));
  }

  [[nodiscard]] auto
  DrawCallWorkerFactory::Create(std::unique_ptr<IDrawCallList>& drawCallList)
    const -> std::unique_ptr<IDrawCallWorker>
  {
    return std::make_unique<DrawCallWorker>(mLogger,
                                            mLoadedScene,
                                            mDrawCallFactory,
                                            drawCallList,
                                            mMeshFactory,
                                            mMeshBufferRequestList,
                                            mAssetDatabase);
  }
}