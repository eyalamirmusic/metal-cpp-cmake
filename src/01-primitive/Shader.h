#pragma once

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(primitiveshaders);

inline const char* getResource(const std::string& name)
{
    auto fs = cmrc::primitiveshaders::get_filesystem();
    auto shader = fs.open(name);
    return shader.begin();
}

inline const char* getShader()
{
    return getResource("Shaders/Shaders.metal");
}

inline const char* getObj()
{
    return getResource("Shaders/bugatti.obj");
}

inline const char* getMaterial()
{
    return getResource("Shaders/bugatti.mtl");
}

