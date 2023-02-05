project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
find_package(Boost 1.80.0 REQUIRED)

add_library(Graphics STATIC 
   "include/Core/Graphics/Window.h"
   "src/Window.cpp" 
   "include/Core/Graphics/GraphicEngineBase.h" 
   "include/Core/Graphics/GraphicEngineDirectX.h" 
   "src/GraphicEngineDirectX.cpp")

set_target_properties(Graphics PROPERTIES 
    FOLDER "Core"
    CXX_STANDARD 20)

target_link_libraries(Graphics
    PUBLIC Boost::boost 
    INTERFACE Lib)

target_include_directories(Graphics 
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PRIVATE "${Boost_INCLUDE_DIRS}")

