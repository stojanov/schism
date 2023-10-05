cmake_minimum_required(VERSION 3.18)
project(schism)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/entt)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    option(ENABLE_SANITIZER "Enable ASAN/LSAN/UBSAN." OFF)
    option(ENABLE_SANITIZER_THREAD "Enable TSAN (does not work together with ASAN)." OFF)

    if(ENABLE_SANITIZER)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=leak")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=leak")
    endif()
    if(ENABLE_SANITIZER_THREAD)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=thread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=thread")
    endif()
    if(ENABLE_SANITIZER AND ENABLE_SANITIZER_THREAD)
        message(FATAL_ERROR "ENABLE_SANITIZER and ENABLE_SANITIZER_THREAD can not be used together!")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    endif()
    add_compile_definitions(SC_DEBUG)
else()
    add_compile_definitions(SC_NODEBUG)
endif()

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

include_directories(
                    vendor/glad/include/
                    vendor/GLFW/include/
                    vendor/glm/glm/
                    vendor/imgui/
                    vendor/spdlog/include/
                    vendor/entt/src
                    vendor/stb/
                    vendor/json/include/
                    vendor/phmap/parallel_hashmap/
                    src/)

file(GLOB STB_SRC vendor/stb/*.c)
file(GLOB GLAD_SOURCE vendor/glad/src/glad.c)
file(GLOB IMGUI_SOURCES vendor/imgui/*.cpp)
file(GLOB IMGUI_HEADERS vendor/imgui/*.h)
file(GLOB IMGUI_GLFW_HEADERS 
                    vendor/imgui/backends/imgui_impl_glfw.h
                    vendor/imgui/backends/imgui_impl_opengl3.h)
file(GLOB IMGUI_GLFW_SOURCES 
                    vendor/imgui/backends/imgui_impl_opengl3.cpp
                    vendor/imgui/backends/imgui_impl_glfw.cpp)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE RESOURCES res/*)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

file(GLOB CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME}
                    ${STB_SRC}
                    ${IMGUI_SOURCES}
                    ${IMGUI_GLFW_SOURCES}
                    ${GLAD_SOURCE} 
                    ${SOURCES} 
                    ${IMGUI_HEADERS}
                    ${HEADERS} 
                    ${IMGUI_GLFW_HEADERS}
                    ${PROJECT_CONFIGS})

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

# Not sure if this will work on linux as the default output bin dir might be different
# will have to test it

# add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD \
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${PROJECT_SOURCE_DIR}/res  
#                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/res)
              
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
    DEPENDS ${RESOURCES})
