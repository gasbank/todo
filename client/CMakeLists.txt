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

############################################################################################################
#########################################     GRPC WORLD    ################################################
############################################################################################################

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Threads REQUIRED)

if(GRPC_AS_SUBMODULE)
  # One way to build a projects that uses gRPC is to just include the
  # entire gRPC project tree via "add_subdirectory".
  # This approach is very simple to use, but the are some potential
  # disadvantages:
  # * it includes gRPC's CMakeLists.txt directly into your build script
  #   without and that can make gRPC's internal setting interfere with your
  #   own build.
  # * depending on what's installed on your system, the contents of submodules
  #   in gRPC's third_party/* might need to be available (and there might be
  #   additional prerequisites required to build them). Consider using
  #   the gRPC_*_PROVIDER options to fine-tune the expected behavior.
  #
  # A more robust approach to add dependency on gRPC is using
  # cmake's ExternalProject_Add (see cmake_externalproject/CMakeLists.txt).
  
  # Include the gRPC's cmake build (normally grpc source code would live
  # in a git submodule called "third_party/grpc", but this example lives in
  # the same repository as gRPC sources, so we just look a few directories up)
  add_subdirectory(../../.. ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
  message(STATUS "Using gRPC via add_subdirectory.")

  # After using add_subdirectory, we can now use the grpc targets directly from
  # this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
  else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
elseif(GRPC_FETCHCONTENT)
  # Another way is to use CMake's FetchContent module to clone gRPC at
  # configure time. This makes gRPC's source code available to your project,
  # similar to a git submodule.
  message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
  include(FetchContent)
  FetchContent_Declare(
    grpc
    GIT_REPOSITORY https://github.com/grpc/grpc.git
    # when using gRPC, you will actually set this to an existing tag, such as
    # v1.25.0, v1.26.0 etc..
    # For the purpose of testing, we override the tag used to the commit
    # that's currently under test.
    GIT_TAG        vGRPC_TAG_VERSION_OF_YOUR_CHOICE)
  FetchContent_MakeAvailable(grpc)

  # Since FetchContent uses add_subdirectory under the hood, we can use
  # the grpc targets directly from this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
else()
  # This branch assumes that gRPC and all its dependencies are already installed
  # on this system, so they can be located by find_package().

  # Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
  set(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)
  message(STATUS "Using protobuf ${protobuf_VERSION}")

  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  set(_REFLECTION gRPC::grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
  else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()

  # Find gRPC installation
  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
  find_package(gRPC CONFIG REQUIRED)
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  set(_GRPC_GRPCPP gRPC::grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  endif()
endif()

# Proto file
get_filename_component(hw_proto "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")

add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")


get_filename_component(tl_proto "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/todolist.proto" ABSOLUTE)
get_filename_component(tl_proto_path "${tl_proto}" PATH)
      
set(tl_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/todolist.pb.cc")
set(tl_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/todolist.pb.h")
set(tl_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/todolist.grpc.pb.cc")
set(tl_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/todolist.grpc.pb.h")

add_custom_command(
      OUTPUT "${tl_proto_srcs}" "${tl_proto_hdrs}" "${tl_grpc_srcs}" "${tl_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${tl_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${tl_proto}"
      DEPENDS "${tl_proto}")
      

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


############################################################################################################
#########################################     GRPC WORLD    ################################################
############################################################################################################









add_executable(NetworkWorld
    main.cpp
    grpcworld.cpp
    todolistclient.cpp
    ${hw_proto_srcs}
    ${hw_grpc_srcs}
    ${tl_proto_srcs}
    ${tl_grpc_srcs}
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

# 테스트용 서버
add_executable(greeter_server
  greeter_server.cc
  ${hw_proto_srcs}
  ${hw_grpc_srcs}
  ${tl_proto_srcs}
  ${tl_grpc_srcs}
)

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

#find_library(GLAD_LIBRARY glad PATHS ${CMAKE_SOURCE_DIR}/glad)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    glad
    SDL2main
    SDL2-static
    lua
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)

target_link_libraries(greeter_server
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
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
