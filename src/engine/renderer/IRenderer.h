#pragma once

// Todo: define some events

class IRenderer {
    public:
        // Setting the target to render to
        virtual void SetRenderBuffer() = 0;

        // Change camera matrix
        virtual void SetCamera() = 0;

        // Submit Geometry to Render
        virtual void Submit() = 0;

        // Render
        virtual void Render() = 0;
};