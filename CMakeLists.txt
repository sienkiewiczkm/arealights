cmake_minimum_required(VERSION 3.8)
project(arealights)

set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)
set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/dependencies/)

set(AREALIGHTS_RESOURCES_DIR ${PROJECT_SOURCE_DIR}/assets CACHE PATH "")

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3)
if (glfw3_FOUND)
  message("GLFW 3.3 found successfully in ${glfw3_DIR}.")
else()
  message("GLFW 3.3 not found. Library will be compiled from sources.")

  if (WIN32)
    set(GLFW_ADDITIONAL_DEFINITIONS "${GLFW_ADDITIONAL_DEFINITIONS} GLFW_EXPOSE_NATIVE_WIN32")
    set(ADDITIONAL_DEFINITIONS "${ADDITIONAL_DEFINITIONS} GLFW_EXPOSE_NATIVE_WIN32")
  endif()

  # glfw: do not build docs, tests and examples
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
  add_subdirectory(${DEPENDENCIES_DIR}glfw)
  target_compile_definitions(glfw PRIVATE ${GLFW_ADDITIONAL_DEFINITIONS})
endif()

#configure_file(${PROJECT_SOURCE_DIR}/configuration/Config.hpp.in ${PROJECT_BINARY_DIR}/configuration/Config.hpp)

set(AREALIGHTS_DEPENDENCIES_SOURCE_FILES
  ${DEPENDENCIES_DIR}/ImGuizmo/ImGuizmo.cpp
  ${DEPENDENCIES_DIR}/easyloggingpp/src/easylogging++.cc
  ${DEPENDENCIES_DIR}/glad-lib/src/glad.c
  ${DEPENDENCIES_DIR}/imgui/imgui.cpp
  ${DEPENDENCIES_DIR}/imgui/imgui_demo.cpp
  ${DEPENDENCIES_DIR}/imgui/imgui_draw.cpp
)

set(AREALIGHTS_SOURCE_FILES
  source/Application.cpp
  source/Common.cpp
  source/DeferredRenderingPipeline.cpp
  source/Effect.cpp
  source/Framework.cpp
  source/FreeCamera.cpp
  source/FreeCameraInputMapper.cpp
  source/ImGuiApplication.cpp
  source/ImGuiBinding.cpp
  source/ImGuiExtensions.cpp
  source/Main.cpp
  source/OpenGLApplication.cpp
  source/OrbitingCamera.cpp
  source/Shaders.cpp
  source/StbImplementation.c
  source/Texture.cpp
  source/TextureUtils.cpp
  source/Vertices.cpp
  source/common/StreamUtils.cpp
  source/inputs/GenericKeyboardInput.cpp
  source/inputs/GenericMouseInput.cpp
)

add_executable(
  ${PROJECT_NAME}
  ${AREALIGHTS_SOURCE_FILES}
  ${AREALIGHTS_DEPENDENCIES_SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${DEPENDENCIES_DIR}/ImGuizmo
  PUBLIC ${DEPENDENCIES_DIR}/easyloggingpp/src
  PUBLIC ${DEPENDENCIES_DIR}/glad-lib/include
  PUBLIC ${DEPENDENCIES_DIR}/glfw/include
  PUBLIC ${DEPENDENCIES_DIR}/glm
  PUBLIC ${DEPENDENCIES_DIR}/imgui
  PUBLIC ${DEPENDENCIES_DIR}/stb
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_BINARY_DIR}/configuration
  ${PROJECT_SOURCE_DIR}/include/fw
)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC ${ADDITIONAL_DEFINITIONS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${OPENGL_gl_LIBRARY}
  assimp
  glfw
)

set(PROJECT_COMPILE_FEATURES
  ${PROJECT_COMPILE_FEATURES}
  cxx_std_14
  cxx_auto_type
  cxx_nullptr
  cxx_range_for
)

target_compile_features(${PROJECT_NAME}
  PUBLIC ${PROJECT_COMPILE_FEATURES}
)
