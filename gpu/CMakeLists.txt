cmake_minimum_required(VERSION 3.17)

project("mandelbruh_opengl")

add_definitions(-DGLEW_STATIC)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    res/Basic.shader
    res/Mandelbrot.shader
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    "D:/dev/lib/glfw-3.3.2.bin.WIN64/include"
    "D:/dev/lib/glew-2.1.0/include"
    "D:/dev/lib/glm/"
    )
        
target_link_libraries(
    ${PROJECT_NAME} 
    "D:/dev/lib/glfw-3.3.2.bin.WIN64/lib-vc2019/glfw3.lib"
    "D:/dev/lib/glew-2.1.0/lib/Release/x64/glew32s.lib"
    opengl32
    )

# set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
