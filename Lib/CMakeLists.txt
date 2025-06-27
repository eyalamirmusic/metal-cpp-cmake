add_library(SharedCodeLib STATIC)

target_include_directories(SharedCodeLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(SharedCodeLib PUBLIC
        METAL_CPP
        GLM
        TinyObjLoader)
target_sources(SharedCodeLib PRIVATE
        SharedCodeLib/App.cpp
        SharedCodeLib/Renderer.cpp
        SharedCodeLib/Models.cpp)