set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

# Libs
link_libraries(Graphics)
find_package(GTest CONFIG REQUIRED)

add_executable(Tests WIN32  "Tests.cpp" "GraphicsTests.h" "LibTests.h")

set_property(TARGET Tests PROPERTY CXX_STANDARD 20)

target_link_libraries(Tests
    PUBLIC Graphics 
    PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_include_directories(Tests
    PUBLIC "${Graphics_SOURCE_DIR}/include") 

set_target_properties(Tests PROPERTIES FOLDER "Tests")
