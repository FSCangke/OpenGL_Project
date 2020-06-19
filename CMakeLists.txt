cmake_minimum_required(VERSION 3.12)
project(OpenGL)

set(CMAKE_CXX_STANDARD 14)

# QT configeration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set (CMAKE_PREFIX_PATH "/usr/local/opt/qt")
find_package(Qt5Widgets REQUIRED)

set(GLEW_H /usr/local/opt/glew/include/GL)
set(GLFW_H /usr/local/opt/glfw/include/GLFW)
set(GLAD_H /usr/local/include/glad)
set(KH_H /usr/local/include/KHR)
set(GLW_H /usr/local/opt/glm/include/glm)
set(FREE_IMAGE_H /usr/local/opt/freeimage/include)
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${GLW_H} ${FREE_IMAGE_H})

# 添加目标链接
set(GLEW_LINK /usr/local/opt/glew/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/opt/glfw/lib/libglfw.3.dylib)
set(FREE_IMAGE_LINK /usr/local/opt/freeimage/lib/libfreeimage.3.18.0.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK} ${FREE_IMAGE_LINK})

# 执行编译命令
# 这个glad.c在第2步中说过要拉到项目中，注意路径
set(SOURCE_FILES "src/glad.c" "src/main.cpp" "src/Bmp.cpp" "src/shaders.cpp")
add_executable(OpenGL ${SOURCE_FILES})

# mac下这步很重要
if (APPLE)
    target_link_libraries(OpenGL "-framework OpenGL")
    target_link_libraries(OpenGL "-framework GLUT")
    target_link_libraries(OpenGL Qt5::Widgets)
endif()
