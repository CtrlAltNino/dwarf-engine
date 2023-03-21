#pragma once

#include "Utilities/Common.h"

#include "Window/Window.h"

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/EditorView.h"
#include "Core/UI/ImGuiLayer.h"
//#include "Core/Rendering/Framebuffer.h"

namespace Dwarf {

    /// @brief The controller part of the editors MVC structure.
    class Editor
    {
    private:
        static Editor* s_Instance;
        /// @brief The MVC model instance of this editor instance.
        Ref<EditorModel> m_Model;

        /// @brief The MVC view instance of this editor instance.
        Ref<EditorView> m_View;

        Scope<Window> m_Window;

        Ref<ImGuiLayer> m_ImguiLayer;

        //Ref<Framebuffer> framebuffer;

        void Init(std::filesystem::path projectPath);
    public:
        Editor();
        ~Editor();

        static Editor& Get() { return *s_Instance; }

        Ref<EditorModel> GetModel(){
            return m_Model;
        }

        /// @brief Starts the render loop
        void Run(std::filesystem::path projectPath);

        void UpdateWindowTitle();
    };

    Editor* CreateEditor();
}