#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Metal uses 0..1 depth
#define GLM_FORCE_LEFT_HANDED // Optional: Metal is right-handed by default
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // Good for matching Metal struct layout
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>