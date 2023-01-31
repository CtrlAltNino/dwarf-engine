#pragma once

#include<glm/vec2.hpp>

class ITexture {
    protected:
        glm::ivec2 size;
    public:
        glm::ivec2 GetSize(){
            return size;
        }
};