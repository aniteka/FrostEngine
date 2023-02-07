project(Lib)

include_directories(${PROJECT_SOURCE_DIR}/include/Core)

# Libs:
set (Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.80.0 COMPONENTS    system 
                                        filesystem 
                                        unit_test_framework 
                                        REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Armadillo CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(Lib INTERFACE 
    "include/core/transformable.h"
    "include/core/movable.h"   
    "include/core/text.h" 
    "include/core/exceptions.h"  
    "include/core/log.h")

set_target_properties(Lib PROPERTIES 
    CXX_STANDARD 20
    LINKER_LANGUAGE CXX
    FOLDER "Core")

target_link_libraries(Lib
    INTERFACE ${Boost_LIBRARIES} ${ARMADILLO_LIBRARIES} spdlog::spdlog spdlog::spdlog_header_only fmt::fmt)
target_include_directories(Lib
    INTERFACE "${Boost_INCLUDE_DIRS}")

