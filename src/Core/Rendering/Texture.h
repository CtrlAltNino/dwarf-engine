#pragma once

#include <glm/vec2.hpp>
#include <filesystem>
#include "Core/Base.h"

namespace Dwarf {
    class Texture {
        protected:
            glm::ivec2 size;
        public:
            glm::ivec2 GetSize(){
                return size;
            }

            static Ref<Texture> Create(std::filesystem::path path);

            virtual uintptr_t GetTextureID()=0;
    };
}