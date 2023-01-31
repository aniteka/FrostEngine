project(Lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
find_package(Boost REQUIRED)
find_package(Armadillo CONFIG REQUIRED)

add_library(Lib STATIC 
    "include/Core/Transformable.h"
     "include/Core/Movable.h"   
     "include/Core/Text.h" 
     "include/Core/Exceptions.h")

set_target_properties(Lib PROPERTIES 
    CXX_STANDARD 20
    LINKER_LANGUAGE CXX
    FOLDER "Core")
if(MSVC)
  target_compile_options(Lib PRIVATE /W4 /WX /analyze /guard:cf)
else()
  target_compile_options(Lib PRIVATE -Wall -Wextra -Wpedantic -Werror -fanalyzer)
endif()

target_link_libraries(Lib
    PUBLIC Boost::boost ${ARMADILLO_LIBRARIES})
target_include_directories(Lib
    PUBLIC "${Boost_INCLUDE_DIRS}")

