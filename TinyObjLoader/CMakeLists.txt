add_library(TinyObjLoader STATIC tiny_obj_loader.cc)
target_include_directories(TinyObjLoader PUBLIC ${CMAKE_CURRENT_LIST_DIR})