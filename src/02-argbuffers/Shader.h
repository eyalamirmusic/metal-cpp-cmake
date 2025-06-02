#pragma once

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(argbuffershaders);

inline const char* getShader()
{
    auto fs = cmrc::argbuffershaders::get_filesystem();
    auto shader = fs.open("Shaders/Shaders.metal");
    return shader.begin();
}