cmake_minimum_required(VERSION 3.10)
project(openglSolar)
#set(CMAKE_PREFIX_PATH "/run/opengl-driver/lib/")
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(OpenGL REQUIRED)
add_subdirectory(lib/glad)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/imgui)
add_executable(
    out
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/planet.cpp
    ${PROJECT_SOURCE_DIR}/src/satellite.cpp
)
target_compile_features(out PRIVATE cxx_std_20)

target_include_directories(
    out
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/lib/
)

target_link_libraries(out glad)
target_link_libraries(out stb_image)
target_link_libraries(out glfw)
target_link_libraries(out imgui)
target_link_libraries(out OpenGL::GL)
