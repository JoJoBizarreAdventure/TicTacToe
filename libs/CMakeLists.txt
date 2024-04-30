set(GLFW_BUILD_WAYLAND OFF)
add_subdirectory(glfw)
function(link_glfw target_name)
    target_link_libraries(${target_name} PRIVATE glfw)
endfunction()

add_subdirectory(lua-cmake)

function(link_lua target_name)
    target_link_libraries(${target_name} PRIVATE lua::lib)
endfunction()

add_subdirectory(glad/cmake)
glad_add_library(glad_gl_core_46 STATIC API gl:core=4.6)
function(link_glad target_name)
    target_link_libraries(${target_name} PRIVATE glad_gl_core_46)
endfunction()

add_subdirectory(glm)
function(link_glm target_name)
    target_link_libraries(${target_name} PRIVATE glm)
endfunction()

add_subdirectory(fmt)
function(link_fmt target_name)
    target_link_libraries(${target_name} PRIVATE fmt)
endfunction()

# build blend2d library for 2d shading
set(BLEND2D_DIR "${CMAKE_CURRENT_SOURCE_DIR}/blend2d"
        CACHE PATH "Location of 'blend2d'")

set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

function(link_blend2d target_name)
    target_link_libraries(${target_name} PRIVATE blend2d::blend2d)
endfunction()