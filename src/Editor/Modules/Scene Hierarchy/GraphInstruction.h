#pragma once

#include "Core/Scene/Scene.h"

namespace Dwarf {

    /// @brief Base graph instruction class.
    class GraphInstruction {
        protected:
            /// @brief Pointer to the currently opened scene.
            Ref<Scene> m_Scene;
        public:
            GraphInstruction(Ref<Scene> scene):m_Scene(scene){}

            /// @brief Executes the instruction.
            virtual void PerformInstruction()=0;
    };
    }