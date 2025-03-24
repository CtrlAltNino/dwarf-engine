#include "pch.h"

#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "DrawCall.h"
#include "DrawCallFactory.h"

namespace Dwarf
{
  DrawCallFactory::DrawCallFactory(
    std::shared_ptr<IDwarfLogger>      logger,
    std::shared_ptr<IMeshBufferWorker> meshBufferWorker)
    : mLogger(std::move(logger))
    , mMeshBufferWorker(std::move(meshBufferWorker))
  {
  }

  auto
  DrawCallFactory::Create(std::unique_ptr<IMesh>& mesh,
                          IMaterial&              material,
                          TransformComponent&     transform)
    -> std::unique_ptr<IDrawCall>
  {
    std::unique_ptr<DrawCall> drawCall =
      std::make_unique<DrawCall>(nullptr, material, transform);

    mMeshBufferWorker->RequestMeshBuffer(std::make_unique<MeshBufferRequest>(
      drawCall->GetMeshBuffer(), mesh->Clone()));

    return std::move(drawCall);
  }
}