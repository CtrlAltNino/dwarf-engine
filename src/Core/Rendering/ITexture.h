#pragma once

#include <glm/vec2.hpp>

namespace Dwarf {
    
    class ITexture {
        protected:
            glm::ivec2 size;
        public:
            glm::ivec2 GetSize(){
                return size;
            }
    };
}