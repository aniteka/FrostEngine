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
    
        "include/core/graphics/shapes/shape_base.h" "src/shape_base.cpp" "include/core/graphics/materials/material_base.h" "src/material_base.cpp" "include/core/graphics/window_event_manager.h"  "src/window_event_manager.cpp")

set_target_properties(Graphics PROPERTIES 
    FOLDER "Core"
    CXX_STANDARD 23)

target_link_libraries(Graphics
    PUBLIC Boost::boost d3d11.lib Microsoft::D3DX11
    INTERFACE Lib)

target_include_directories(Graphics 
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PRIVATE "${Boost_INCLUDE_DIRS}")

