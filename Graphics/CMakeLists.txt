project(Graphics)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Graphics)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
# link_libraries(Lib)
find_package(Boost COMPONENTS log REQUIRED)

add_library(Graphics STATIC 
   "include/Core/Graphics/Window.h"
   "src/Window.cpp" 
   "include/Core/Graphics/GraphicEngineOpenGL.h" 
   "src/GraphicEngineOpenGL.cpp" 
   "include/Core/Graphics/GraphicEngineBase.h")

set_target_properties(Graphics PROPERTIES 
    FOLDER "Core"
    CXX_STANDARD 20)
if(MSVC)
  target_compile_options(Graphics PRIVATE /W4 /WX /analyze /guard:cf)
else()
  target_compile_options(Graphics PRIVATE -Wall -Wextra -Wpedantic -Werror -fanalyzer)
endif()

target_link_libraries(Graphics
    PRIVATE Boost::boost Boost::log)

target_include_directories(Graphics
    PUBLIC "${Lib_SOURCE_DIR}/include"
    PUBLIC "${Boost_INCLUDE_DIRS}")

