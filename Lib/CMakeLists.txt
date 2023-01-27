project(Lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Libs:
find_package(Boost REQUIRED)
find_package(Armadillo CONFIG REQUIRED)

add_library(Lib STATIC 
    "include/Core/Transformable.h"
     "include/Core/Movable.h" )

set_property(TARGET Lib PROPERTY CXX_STANDARD 20)
set_property(TARGET Lib PROPERTY LINKER_LANGUAGE CXX)

target_link_libraries(Lib
    PUBLIC Boost::boost ${ARMADILLO_LIBRARIES})
target_include_directories(Lib
    PUBLIC "${Boost_INCLUDE_DIRS}")

# Cosmetic:
set_target_properties(Lib PROPERTIES FOLDER "Core")
