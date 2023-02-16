#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <filesystem>

#include "Core/Rendering/ITexture.h"

namespace Dwarf {
    
    class MetalTexture : ITexture {
        private:
            MTL::Texture* texture;
        public:
            MetalTexture(std::filesystem::path path);
            MTL::Texture* GetMetalTexture();
    };
}