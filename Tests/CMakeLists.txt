set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

# Libs
link_libraries(Graphics)
set (Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.80.0 COMPONENTS    system 
                                        filesystem 
                                        unit_test_framework 
                                        REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(Tests "Tests.cpp" "GraphicsTests.h" )

set_property(TARGET Tests PROPERTY CXX_STANDARD 20)

target_link_libraries(Tests
    PUBLIC Graphics 
    PRIVATE ${Boost_LIBRARIES})
target_include_directories(Tests
    PUBLIC "${Graphics_SOURCE_DIR}/include") 

set_target_properties(Tests PROPERTIES FOLDER "Tests")

add_test(NAME tests COMMAND Tests)
