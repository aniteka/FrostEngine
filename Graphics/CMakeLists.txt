project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)

# Libs:
link_libraries(Lib)
find_package(Vulkan REQUIRED)
find_package(Boost  REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(Graphics STATIC 
    "src/Graphics.cpp" 
    "include/Core/Graphics/Graphics.h")

set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)

target_link_libraries(Graphics
    PRIVATE glfw 
    PRIVATE Vulkan::Vulkan)

target_include_directories(Graphics
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PUBLIC "${Vulkan_INCLUDE_DIR}"
    PUBLIC "${GLFW_INCLUDE_DIRS}"
    PUBLIC "${Boost_INCLUDE_DIRS}")

# Cosmetic:
set_target_properties(Graphics PROPERTIES FOLDER "Core")
