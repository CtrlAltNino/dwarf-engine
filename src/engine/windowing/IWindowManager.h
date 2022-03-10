#pragma once

#include"../../utilities/Common.h"
#include"../rendering/IRenderTexture.h"
#include<vector>
#include<glm/vec4.hpp>
#include<string>

#define INITIAL_WINDOW_WIDTH (1280)
#define INITIAL_WINDOW_HEIGHT (720)

class IWindowManager {
    protected:
        glm::vec4 clearColor = glm::vec4(0.1,0.1,0.1,1);
        int width;
        int height;
        std::vector<IRenderTexture*> renderTextures;
    public:
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