#pragma once

namespace Dwarf
{
  class IAssetInspector
  {
  public:
    virtual ~IAssetInspector() = default;
    virtual void
    Render(const std::filesystem::path& assetPath) const = 0;
  };
}