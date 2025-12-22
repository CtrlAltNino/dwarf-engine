#include "pch.hpp"

#include "ShadowMapper.hpp"

#include <utility>

namespace Dwarf
{
  ShadowMapper::ShadowMapper(std::shared_ptr<ILoadedScene> loadedScene)
    : mLoadedScene(std::move(loadedScene))
  {
  }

  void
  ShadowMapper::Update()
  {
    // Check Directional light

    // Point lights
  }

  [[nodiscard]] auto
  ShadowMapper::GetFrameData() const -> const ShadowFrameData
  {
  }

  void
  ShadowMapper::OnShadowSettingsChanged()
  {
  }
}