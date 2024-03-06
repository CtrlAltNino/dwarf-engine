#pragma once

#include "Window/Window.h"

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/EditorView.h"
#include "Core/UI/ImGuiLayer.h"

namespace Dwarf
{

    /// @brief The controller part of the editors MVC structure.
    class Editor
    {
    private:
        static Ref<Editor> s_Instance;
        /// @brief The MVC model instance of this editor instance.
        Ref<EditorModel> m_Model;

        /// @brief The MVC view instance of this editor instance.
        Ref<EditorView> m_View;

        Ref<Window> m_Window;

        Ref<ImGuiLayer> m_ImguiLayer;

        void Init(std::filesystem::path const &projectPath);

    public:
        Editor();
        ~Editor();

        static Ref<Editor> Get() { return s_Instance; }

        Ref<EditorModel> GetModel() const
        {
            return m_Model;
        }

        /// @brief Starts the render loop
        bool Run(std::filesystem::path const &projectPath);

        void UpdateWindowTitle() const;
    };

    Ref<Editor> CreateEditor();
}