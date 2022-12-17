project(Lib)

include_directories(${PROJECT_SOURCE_DIR}/include/Core/Lib)

# Libs:
find_package(Boost 1.80.0 REQUIRED)

add_library(Lib STATIC 
    "src/Lib.cpp" 
    "include/Core/Lib/Lib.h")

set_property(TARGET Lib PROPERTY CXX_STANDARD 20)

target_include_directories(Lib
    PUBLIC "${Boost_INCLUDE_DIRS}")

# Cosmetic:
set_target_properties(Lib PROPERTIES FOLDER "Core")
