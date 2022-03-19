#pragma once

#include"../../utilities/Common.h"
#include"../rendering/IRenderTexture.h"
#include<imgui_internal.h>
#include<vector>
#include<glm/vec4.hpp>
#include<string>
#include<map>

#define INITIAL_WINDOW_WIDTH (1280)
#define INITIAL_WINDOW_HEIGHT (720)
#define INTER_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define INTER_BOLD_PATH "data/engine/fonts/Inter-Bold.ttf"

class IWindowManager {
    protected:
        glm::vec4 clearColor = glm::vec4(0.1,0.1,0.1,1);
        int width;
        int height;
        std::vector<IRenderTexture*> renderTextures;
    public:
        static inline std::map<std::string,ImFont*> fonts;
        static inline std::string rendererName;
        static inline std::string vendorName;
        static inline std::string apiVersion;
        virtual void Init()=0;
        virtual GraphicsApi GetActiveApi()=0;
        virtual void CreateEditorWindow()=0;
        virtual void SetWindowName(std::string windowName)=0;
        virtual void StartFrame()=0;
        virtual void EndFrame()=0;
        virtual int GetWidth()=0;
        virtual int GetHeight()=0;
        virtual bool ShouldWindowCloseSignal()=0;
        virtual void InitImGui()=0;
        virtual IRenderTexture* AddRenderTexture()=0;
        virtual void RemoveRenderTexture(int ID)=0;
        virtual std::vector<IRenderTexture*>* GetRenderTextures() {return &renderTextures;}
};