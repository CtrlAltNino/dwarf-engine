#include "Core/UI/ImGuiLayer.h"
#ifdef _WIN32
    #include "Platform/OpenGL/OpenGLImGuiLayer.h"
#elif __APPLE__
    
#elif __linux__
    
#endif

namespace Dwarf {
    Ref<ImGuiLayer> ImGuiLayer::Create(GraphicsApi api){
        #ifdef _WIN32
            switch(api){
                case GraphicsApi::D3D11: break;
                case GraphicsApi::D3D12: break;
                case GraphicsApi::OpenGL:
                        return CreateRef<OpenGLImGuiLayer>();
                    break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::Vulkan: break;
            }
        #elif __APPLE__
            switch(api){
                case GraphicsApi::D3D11: break;
                case GraphicsApi::D3D12: break;
                case GraphicsApi::OpenGL: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::Vulkan: break;
            }
        #elif __linux__
            switch(api){
                case GraphicsApi::D3D11: break;
                case GraphicsApi::D3D12: break;
                case GraphicsApi::OpenGL: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::Vulkan: break;
            }
        #endif

        return nullptr;
    }
}