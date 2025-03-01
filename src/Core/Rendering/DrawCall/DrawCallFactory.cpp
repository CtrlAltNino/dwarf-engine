#include "DrawCallFactory.h"
#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "DrawCall.h"

namespace Dwarf
{
  DrawCallFactory::DrawCallFactory(
    std::shared_ptr<IDwarfLogger>      logger,
    std::shared_ptr<IMeshBufferWorker> meshBufferWorker)
    : m_Logger(logger)
    , m_MeshBufferWorker(meshBufferWorker)
  {
  }

  std::unique_ptr<IDrawCall>
  DrawCallFactory::Create(std::unique_ptr<IMesh>& mesh,
                          IMaterial&              material,
                          TransformComponent&     transform)
  {
    std::unique_ptr<DrawCall> drawCall =
      std::make_unique<DrawCall>(nullptr, material, transform);

    m_MeshBufferWorker->RequestMeshBuffer(std::make_unique<MeshBufferRequest>(
      drawCall->GetMeshBuffer(), mesh->Clone()));

    return std::move(drawCall);
  }
}