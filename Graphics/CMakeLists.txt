project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
# link_libraries(Lib)
find_package(Boost COMPONENTS log REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(Graphics STATIC 
   "include/Core/Graphics/Window.h"
   "src/Window.cpp" 
   "include/Core/Graphics/GraphicEngineOpenGL.h" 
   "src/GraphicEngineOpenGL.cpp" 
   "include/Core/Graphics/GraphicEngineBase.h")

set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)

target_link_libraries(Graphics
    PRIVATE glfw Boost::boost Boost::log)

target_include_directories(Graphics
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PUBLIC "${GLFW_INCLUDE_DIRS}"
    PUBLIC "${Boost_INCLUDE_DIRS}")

# Cosmetic:
set_target_properties(Graphics PROPERTIES FOLDER "Core")
