set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(GraphicsTests "graphic.cpp")
target_link_libraries(GraphicsTests
    PUBLIC Graphics)
target_include_directories(GraphicsTests
    PUBLIC "${Graphics_SOURCE_DIR}/include") 

set_target_properties(GraphicsTests PROPERTIES FOLDER "Tests")
