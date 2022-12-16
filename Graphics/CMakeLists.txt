project(Graphics)

link_libraries(Lib)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)

find_package(Vulkan REQUIRED)
find_package(Boost 1.80.0 REQUIRED)

add_library(Graphics STATIC 
    "src/Graphics.cpp" 
    "include/Core/Graphics/Graphics.h")

set_target_properties(Graphics PROPERTIES FOLDER "Core")

set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)

target_link_libraries(Graphics
    PRIVATE glfw
    PRIVATE Vulkan::Vulkan)

target_include_directories(Graphics
    PUBLIC "${GLFW_SOURCE_DIR}/include"
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PUBLIC "${Vulkan_INCLUDE_DIRS}"
    PUBLIC "${Boost_INCLUDE_DIRS}")
