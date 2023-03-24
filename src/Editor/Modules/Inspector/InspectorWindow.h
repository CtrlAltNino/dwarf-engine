#pragma once

#include "Core/Scene/Scene.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Core/UI/DwarfUI.h"

#define _USE_MATH_DEFINES
#include<cmath>

namespace Dwarf {

    /// @brief Module that renders a window, containing information of selected objects or assets.
    class InspectorWindow : public GuiModule{
        private:
            /// @brief Pointer to the currently opened scene instance.
            Ref<Scene> m_Scene;

            /// @brief Renders the components of an entity.
            /// @param entity Entity to render in the inspector.
            void RenderComponents(Entity entity);

            void BeginComponent(const char* componentHeader);

            void EndComponent();

            /*void RenderTagComponent(TagComponent component);
            void RenderTransformComponent(TransformComponent component);
            void RenderPathComponent(PathComponent component);
            void RenderLightComponent(LightComponent component);
            void RenderMeshRendererComponent(MeshRendererComponent component);*/
            
            template <typename T>
            void RenderComponent(T* component); 

            template <typename T>
            void RenderComponent(Ref<T> component); 
        public:
            InspectorWindow(Ref<EditorModel> listener, int id);

            /// @brief Renders the module window.
            void OnImGuiRender() override;

            void OnUpdate(double deltaTime) override;
    };
}