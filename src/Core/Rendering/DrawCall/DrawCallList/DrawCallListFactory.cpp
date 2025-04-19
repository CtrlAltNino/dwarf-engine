#include "pch.hpp"

#include "Core/Rendering/DrawCall/DrawCallList/DrawCallList.hpp"
#include "DrawCallListFactory.hpp"

namespace Dwarf
{
  DrawCallListFactory::DrawCallListFactory(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(
      Log("DrawCallListFactory created", "DrawCallListFactory"));
  }

  DrawCallListFactory::~DrawCallListFactory()
  {
    mLogger->LogDebug(
      Log("DrawCallListFactory destroyed", "DrawCallListFactory"));
  }

  [[nodiscard]] auto
  DrawCallListFactory::Create() const -> std::unique_ptr<IDrawCallList>
  {
    return std::make_unique<DrawCallList>();
  }
}