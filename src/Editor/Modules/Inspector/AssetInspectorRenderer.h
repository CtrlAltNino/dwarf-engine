#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"

namespace Dwarf
{
    class AssetInspectorRenderer
    {
    private:
        static Ref<EditorModel> s_Model;

    public:
        static void Init(Ref<EditorModel> model);
        template <typename T>
        static void RenderAssetInspector(T *asset);

        template <typename T>
        static void RenderAssetInspector(Ref<T> component);
    };
}