function(add_project project)
    file(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/${project}/*.cpp)
    add_executable(${project} ${sources})
    target_link_libraries(${project} PRIVATE SharedCodeLib)
endfunction()

add_project(00-window)