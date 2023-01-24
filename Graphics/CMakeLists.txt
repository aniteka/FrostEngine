project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)

# Libs:
# link_libraries(Lib)
find_package(Boost COMPONENTS log REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(Graphics STATIC 
   "include/Core/Graphics/Window.h"
   "src/Window.cpp")

set_property(TARGET Graphics PROPERTY CXX_STANDARD 17)

target_link_libraries(Graphics
    PRIVATE glfw Boost::boost Boost::log)

target_include_directories(Graphics
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PUBLIC "${GLFW_INCLUDE_DIRS}"
    PUBLIC "${Boost_INCLUDE_DIRS}")

# Cosmetic:
set_target_properties(Graphics PROPERTIES FOLDER "Core")
