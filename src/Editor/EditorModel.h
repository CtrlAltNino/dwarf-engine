#pragma once

#include "Core/Base.h"

#include <string_view>

#include "Core/Scene/Scene.h"

namespace Dwarf
{

    enum class INSPECTOR_SELECTION_TYPE
    {
        NONE,
        ASSET,
        ENTITY
    };

    struct SelectionContainer
    {
        std::filesystem::path assetPath;
        std::vector<Entity> selectedEntities;
        INSPECTOR_SELECTION_TYPE selectionType = INSPECTOR_SELECTION_TYPE::NONE;
    };

    /// @brief Model of the Editor's MVC structure.
    class EditorModel
    {
    private:
        /// @brief The currently opened scene instance.
        Ref<Scene> m_Scene;
        double m_DeltaTime;
        std::string m_ProjectName;
        std::filesystem::path m_ProjectPath;
        bool m_ReturnToLauncher = false;
        bool m_CloseSignal = false;
        SelectionContainer m_Selection;

    public:
        EditorModel(std::string_view name, std::filesystem::path const &projectPath);

        /// @brief Returns the currently opened scene.
        /// @return A pointer to the scene.
        Ref<Scene> GetScene() const;

        void SetScene(Ref<Scene> scene);

        std::string GetName() const;

        std::filesystem::path GetProjectPath() const;

        void SetDeltaTime(double deltaTime);

        double GetDeltaTime() const;

        bool GetCloseSignal() const;

        bool GetReturnToLauncher() const;

        SelectionContainer &GetSelection();

        void CloseEditor(bool returnToLauncher);
    };
}