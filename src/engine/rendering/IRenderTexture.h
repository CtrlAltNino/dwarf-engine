#pragma once

#include<glm/vec2.hpp>

enum class RENDER_MODE {FREE, RESOLUTION, ASPECT_RATIO};

class IRenderTexture {
    protected:
        int textureWidth = 512;
        int textureHeight = 512;
        float aspectRatio = 1;
        RENDER_MODE renderMode = RENDER_MODE::FREE;
    public:
        virtual void Bind()=0;
        virtual void Unbind()=0;

        virtual int GetID()=0;
        virtual int GetTexture()=0;

        RENDER_MODE GetRenderMode(){
            return renderMode;
        }
        
        glm::vec2 GetResolution() {
            return glm::vec2(textureWidth, textureHeight);
        }
        
        float GetAspectRatio() {
            return aspectRatio;
        }

        virtual void UpdateTextureResolution()=0;
        
        void SetResolution(int width, int height){
            textureWidth = width;
            textureHeight = height;
            UpdateTextureResolution();
        }

        void SetAspectRatio(float aspectRatio){
            aspectRatio = aspectRatio;
            UpdateTextureResolution();
        }

        void SetRenderMode(RENDER_MODE renderMode){
            renderMode = renderMode;
            UpdateTextureResolution();
        }
};