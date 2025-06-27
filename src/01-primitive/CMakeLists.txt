add_executable(01-primitive 01-primitive.cpp)
cmrc_add_resource_library(primitiveshaders
        Shaders/Shaders.metal
        Shaders/bugatti.obj
        Shaders/bugatti.mtl)
target_link_libraries(01-primitive PRIVATE SharedCodeLib primitiveshaders)
