cmake_minimum_required(VERSION 3.20)
project(LSYS)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/glad)
include_directories(include/GLFW)
include_directories(include/glm)
include_directories(include/glm/detail)
include_directories(include/glm/ext)
include_directories(include/glm/gtc)
include_directories(include/glm/gtx)
include_directories(include/glm/simd)
include_directories(include/KHR)

# Include Dear ImGui directories
include_directories(
        lib/imgui
        lib/imgui/backends
)

add_executable(
    LSYS
        src/rendering/glad.c
        src/main.cpp
        src/rendering/ShaderObject.cpp
        src/rendering/ShaderProgram.cpp
        src/utility/logging.cpp
        src/lindenmayer/lindenmayer.cpp
        src/utility/Transform.cpp
        src/utility/Turtle.cpp
        src/UI/UIManager.cpp
        src/rendering/Camera.cpp
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        src/UI/UIManager.cpp
        src/rendering/ShaderManager.cpp
        include/rendering/RenderableBase.h
        src/rendering/RenderableBase.cpp
        src/rendering/RenderingContext.cpp
        src/utility/util.cpp
        include/utility/util.h
)


set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/lib-mingw-w64")

if(WIN32)
    target_link_libraries(LSYS PRIVATE
            "${CMAKE_SOURCE_DIR}/lib/lib-mingw-w64/libglfw3.a"
            -static)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(LSYS PRIVATE glfw GL dl)
endif()

add_custom_command(TARGET LSYS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resource
        $<TARGET_FILE_DIR:LSYS>/resource)