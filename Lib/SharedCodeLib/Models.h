#pragma once

#include "Graphics.h"
#include <string>

namespace Graphics
{
struct Model
{
    Vertices positions;
    Vertices colors;
};

Model loadModelFromObjData(const std::string& objData,
                           const std::string& mtlData = "");

} // namespace Graphics