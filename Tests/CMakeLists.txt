set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

# Libs
link_libraries(Graphics)
set (Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.80.0 COMPONENTS    system 
                                        filesystem 
                                        unit_test_framework 
                                        REQUIRED)
find_package(dxsdk-d3dx CONFIG REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(Tests "tests.cpp" "graphics_tests.h" )

set_property(TARGET Tests PROPERTY CXX_STANDARD 20)

target_link_libraries(Tests
    PUBLIC Graphics 
    PRIVATE ${Boost_LIBRARIES} d3d11.lib Microsoft::D3DX11)
target_include_directories(Tests
    PUBLIC "${Graphics_SOURCE_DIR}/include") 

set_target_properties(Tests PROPERTIES FOLDER "Tests")

add_test(NAME tests COMMAND Tests)
