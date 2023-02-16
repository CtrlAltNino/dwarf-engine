#pragma once

#include "Core/Scene/Scene.h"

namespace Dwarf {

    /// @brief Base graph instruction class.
    class GraphInstruction {
        protected:
            /// @brief Pointer to the currently opened scene.
            Scene* scene;
        public:
            GraphInstruction(Scene* scene):scene(scene){}

            /// @brief Executes the instruction.
            virtual void PerformInstruction()=0;
    };
    }