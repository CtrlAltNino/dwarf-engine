#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <filesystem>

#include "Core/Base.h"
#include "Core/Rendering/ITexture.h"

namespace Dwarf {
    
    class MetalTexture : public ITexture {
        private:
            MTL::Texture* texture;
        public:
            MetalTexture(std::filesystem::path path);
            ~MetalTexture();
            MTL::Texture* GetMetalTexture();
            void* Get();
    };
}