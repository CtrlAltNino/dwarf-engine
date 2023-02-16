#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Core/Rendering/MetalTexture.h"

namespace Dwarf {

    MetalTexture::MetalTexture(std::filesystem::path path){
        int numColCh;
        unsigned char* bytes = stbi_load(path.c_str(), &this->size.x, &this->size.y, &numColCh, 0);

        //const uint32_t tw = 128;
        //const uint32_t th = 128;

        MTL::TextureDescriptor* pTextureDesc = MTL::TextureDescriptor::alloc()->init();
        pTextureDesc->setWidth( this->size.x );
        pTextureDesc->setHeight( this->size.y );

        const char* extension = path.extension().c_str();

        if(std::strcmp(extension, "jpg") == 0) {
                pTextureDesc->setPixelFormat( MTL::PixelFormatETC2_RGB8 );
        } else if(std::strcmp(extension, "jpg") == 0){
                pTextureDesc->setPixelFormat( MTL::PixelFormatRGBA8Unorm );
        }

        pTextureDesc->setTextureType( MTL::TextureType2D );
        pTextureDesc->setStorageMode( MTL::StorageModeManaged );
        pTextureDesc->setUsage( MTL::ResourceUsageSample | MTL::ResourceUsageRead );

        MTL::Device* device = MTL::CreateSystemDefaultDevice();
        MTL::Texture *pTexture = device->newTexture( pTextureDesc );
        texture = pTexture;

        //uint8_t* pTextureData = (uint8_t *)alloca( tw * th * 4 );

        texture->replaceRegion( MTL::Region( 0, 0, 0, this->size.x, this->size.y, 1 ), 0, bytes, this->size.x * 4 );

        pTextureDesc->release();
    }

    MTL::Texture* MetalTexture::GetMetalTexture(){
        return this->texture;
    }
}