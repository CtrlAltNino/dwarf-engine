#pragma once

#include "Core/Base.h"

#include "Core/Scene/Scene.h"

namespace Dwarf {

    enum INSPECTOR_SELECTION_TYPE { NONE, ASSET, ENTITY };

    struct SelectionContainer {
        std::filesystem::path assetPath;
        std::vector<Entity> selectedEntities;
        INSPECTOR_SELECTION_TYPE selectionType = INSPECTOR_SELECTION_TYPE::NONE;
    };

    /// @brief Model of the Editor's MVC structure.
    class EditorModel{
        private:
            /// @brief The currently opened scene instance.
            Ref<Scene> m_Scene;

            double m_DeltaTime;

            std::filesystem::path m_ProjectPath;

            std::string m_ProjectName;
        public:
            EditorModel(std::string name, std::filesystem::path projectPath);

            /// @brief Returns the currently opened scene.
            /// @return A pointer to the scene.
            Ref<Scene> GetScene();

            void SetScene(Ref<Scene> scene);

            std::string GetName();

            std::filesystem::path GetProjectPath();

            void SetDeltaTime(double deltaTime);

            double GetDeltaTime();

            SelectionContainer m_Selection;
    };
}