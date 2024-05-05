#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"

namespace Dwarf
{
  class AssetInspectorRenderer
  {
  private:
    static std::shared_ptr<EditorModel> s_Model;

  public:
    static void
    Init(std::shared_ptr<EditorModel> model);
    template<typename T>
    static void
    RenderAssetInspector(std::shared_ptr<T> asset);
  };
}