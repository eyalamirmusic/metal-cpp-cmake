#pragma once

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(primitiveshaders);

inline const char* getShader()
{
    auto fs = cmrc::primitiveshaders::get_filesystem();
    auto shader = fs.open("Shaders/Shaders.metal");
    return shader.begin();
}