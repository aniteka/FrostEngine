project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/core/graphics)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
find_package(Boost 1.80.0 REQUIRED)
find_package(dxsdk-d3dx CONFIG REQUIRED)

add_library(Graphics STATIC 
   "include/core/graphics/window.h"
   "src/window.cpp" 
   "include/core/graphics/graphic_engine_base.h" 
   "include/core/graphics/graphic_engine_directx.h" 
   "src/graphic_engine_directx.cpp" 
   "include/core/graphics/shapes/simple_triangle2d.h" 
   "src/simple_triangle2d.cpp" "include/core/graphics/shapes/drawable.h")

set_target_properties(Graphics PROPERTIES 
    FOLDER "Core"
    CXX_STANDARD 20)

target_link_libraries(Graphics
    PUBLIC Boost::boost d3d11.lib Microsoft::D3DX11
    INTERFACE Lib)

target_include_directories(Graphics 
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PRIVATE "${Boost_INCLUDE_DIRS}")

