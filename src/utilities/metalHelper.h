#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <QuartzCore/CAMetalLayer.h>

void assign_device(void* layer, MTL::Device* device);

CA::MetalDrawable* next_drawable(void* layer);