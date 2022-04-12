#pragma once

// Todo: define some events

class IRenderer {
    public:
        virtual void SetRenderBuffer() = 0;
        virtual void SetCamera() = 0;
        virtual void Submit() = 0;
        virtual void Render() = 0;
};