#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <filesystem>

#include "Core/Base.h"
#include "Core/Rendering/ITexture.h"

namespace Dwarf {
    
    class MetalTexture : ITexture {
        private:
            Ref<MTL::Texture> texture;
        public:
            MetalTexture(std::filesystem::path path);
            Ref<MTL::Texture> GetMetalTexture();
            void* Get();
    };
}