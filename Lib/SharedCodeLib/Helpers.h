#pragma once

#include <Metal/Metal.hpp>
#include <iostream>

namespace Apple
{
inline NS::SharedPtr<MTL::Function> createFunction(MTL::Library* lib,
                                            const std::string& name)
{
    auto func =
        NS::String::string(name.c_str(), NS::StringEncoding::UTF8StringEncoding);

    return NS::TransferPtr(lib->newFunction(func));
}

inline NS::SharedPtr<MTL::Library> createLibrary(MTL::Device* device,
                                                 const std::string& shader)
{
    NS::Error* error = nullptr;
    auto* lib = device->newLibrary(
        NS::String::string(shader.c_str(), NS::StringEncoding::UTF8StringEncoding),
        nullptr,
        &error);

    if (!lib)
    {
        std::cout << error->localizedDescription()->utf8String() << std::endl;
        assert(false);
    }

    return NS::TransferPtr(lib);
}

inline NS::SharedPtr<MTL::RenderPipelineState>
    createRenderState(MTL::Device* device, MTL::RenderPipelineDescriptor* desc)
{
    NS::Error* error = nullptr;
    auto pso = device->newRenderPipelineState(desc, &error);

    if (!pso)
    {
        std::cout << error->localizedDescription()->utf8String() << std::endl;
        assert(false);
    }

    return NS::TransferPtr(pso);
}
} // namespace Apple