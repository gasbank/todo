cmake_minimum_required(VERSION 3.10)

project(NetworkWorld)


set(CMAKE_CXX_STANDARD 17)

# 이게 없으면 Xcode Archive 시 링크 에러가 난다.
# https://stackoverflow.com/questions/30318941/cmake-generated-xcode-project-release-build-works-but-archive-fails-on-linker
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/library )

add_subdirectory(glad)
add_subdirectory(SDL2)
add_subdirectory(lua)

include_directories("glad/include")
include_directories("SDL2/include")
include_directories("lua/src")

find_package(OpenGL REQUIRED)

add_executable(NetworkWorld
    main.cpp
    luaconsole.cpp
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_sdl.cpp
    imgui/imgui_impl_sdl.h
)

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

#find_library(GLAD_LIBRARY glad PATHS ${CMAKE_SOURCE_DIR}/glad)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    glad
    SDL2main
    SDL2-static
    lua
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Hack-Regular.ttf
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Hack-Regular.ttf
            ${CMAKE_CURRENT_BINARY_DIR}/Hack-Regular.ttf
    DEPENDS ${CMAKE_SOURCE_DIR}/Hack-Regular.ttf
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mood.lua
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/mood.lua
            ${CMAKE_CURRENT_BINARY_DIR}/mood.lua
    DEPENDS ${CMAKE_SOURCE_DIR}/mood.lua
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/inspect.lua
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/inspect.lua
            ${CMAKE_CURRENT_BINARY_DIR}/inspect.lua
    DEPENDS ${CMAKE_SOURCE_DIR}/inspect.lua
)

add_custom_target(copy_fonts DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Hack-Regular.ttf")
add_custom_target(copy_lua DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mood.lua")
add_custom_target(copy_inspect DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/inspect.lua")

add_dependencies(NetworkWorld copy_fonts copy_lua copy_inspect)

SET_TARGET_PROPERTIES(NetworkWorld PROPERTIES MACOSX_BUNDLE TRUE)
