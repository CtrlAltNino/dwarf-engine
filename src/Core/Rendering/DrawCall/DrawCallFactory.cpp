#include "pch.hpp"

#include "Core/Rendering/MeshBuffer/MeshBufferRequestList/IMeshBufferRequestList.hpp"
#include "DrawCall.hpp"
#include "DrawCallFactory.hpp"
#include <memory>

namespace Dwarf
{
  DrawCallFactory::DrawCallFactory(
    std::shared_ptr<IDwarfLogger>           logger,
    std::shared_ptr<IMeshBufferRequestList> MeshBufferRequestList)
    : mLogger(std::move(logger))
    , mMeshBufferRequestList(std::move(MeshBufferRequestList))
  {
    mLogger->LogDebug(Log("DrawCallFactory created", "DrawCallFactory"));
  }

  DrawCallFactory::~DrawCallFactory()
  {
    mLogger->LogDebug(Log("DrawCallFactory destroyed", "DrawCallFactory"));
  }

  auto
  DrawCallFactory::Create(std::shared_ptr<IMesh>&  mesh,
                          MaterialAsset&           material,
                          TransformComponentHandle transform)
    -> std::shared_ptr<IDrawCall>
  {
    std::shared_ptr<DrawCall> drawCall =
      std::make_shared<DrawCall>(nullptr, material, transform);

    mMeshBufferRequestList->RequestMeshBuffer(
      std::make_unique<MeshBufferRequest>(
        [drawCall =
           std::weak_ptr(drawCall)](std::unique_ptr<IMeshBuffer>&& buffer)
        {
          if (auto drawCallShared = drawCall.lock())
          {
            drawCallShared->SetMeshBuffer(std::move(buffer));
          }
        },
        mesh->Clone()));

    return drawCall;
  }
}