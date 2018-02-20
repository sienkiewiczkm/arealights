cmake_minimum_required(VERSION 3.8)
project(arealights)

set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)
set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/dependencies/)
set(AREALIGHTS_RESOURCES_DIR ${PROJECT_SOURCE_DIR}/assets CACHE PATH "")

find_package(OpenGL REQUIRED)

find_package(glfw3 3.2)
if (glfw3_FOUND)
  message("GLFW 3.2 found successfully in ${glfw3_DIR}.")
else ()
  message("GLFW 3.2 not found. Library will be compiled from sources.")

  if (WIN32)
    set(GLFW_ADDITIONAL_DEFINITIONS "${GLFW_ADDITIONAL_DEFINITIONS} GLFW_EXPOSE_NATIVE_WIN32")
    set(ADDITIONAL_DEFINITIONS "${ADDITIONAL_DEFINITIONS} GLFW_EXPOSE_NATIVE_WIN32")
  endif ()

  # glfw: do not build docs, tests and examples
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
  add_subdirectory(${DEPENDENCIES_DIR}glfw)
  target_compile_definitions(glfw PRIVATE ${GLFW_ADDITIONAL_DEFINITIONS})
endif ()

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
  source/Application.hpp
  source/DeferredRenderingPipeline.cpp
  source/DeferredRenderingPipeline.hpp
  source/FreeCameraInputMapper.cpp
  source/FreeCameraInputMapper.hpp
  source/Main.cpp
  source/Material.cpp
  source/Material.hpp
  source/PolygonalLight.hpp
  source/RenderHelper.cpp
  source/RenderHelper.hpp
  source/framework/BasicMeshes.cpp
  source/framework/BasicMeshes.hpp
  source/framework/Common.cpp
  source/framework/Common.hpp
  source/framework/Effect.cpp
  source/framework/Effect.hpp
  source/framework/Framework.cpp
  source/framework/Framework.hpp
  source/framework/FreeCamera.cpp
  source/framework/FreeCamera.hpp
  source/framework/ImGuiApplication.cpp
  source/framework/ImGuiApplication.hpp
  source/framework/ImGuiBinding.cpp
  source/framework/ImGuiBinding.hpp
  source/framework/ImGuiExtensions.cpp
  source/framework/ImGuiExtensions.hpp
  source/framework/Logging.hpp
  source/framework/Mesh.hpp
  source/framework/OpenGL.hpp
  source/framework/OpenGLApplication.cpp
  source/framework/OpenGLApplication.hpp
  source/framework/OrbitingCamera.cpp
  source/framework/OrbitingCamera.hpp
  source/framework/Shaders.cpp
  source/framework/Shaders.hpp
  source/framework/StbImplementation.c
  source/framework/Texture.cpp
  source/framework/Texture.hpp
  source/framework/TextureUtils.cpp
  source/framework/TextureUtils.hpp
  source/framework/Vertices.cpp
  source/framework/Vertices.hpp
  source/framework/common/Exceptions.hpp
  source/framework/common/StreamUtils.cpp
  source/framework/common/StreamUtils.hpp
  source/framework/inputs/GenericKeyboardInput.cpp
  source/framework/inputs/GenericKeyboardInput.hpp
  source/framework/inputs/GenericMouseInput.cpp
  source/framework/inputs/GenericMouseInput.hpp
  source/framework/inputs/IKeyboardInput.hpp
  source/framework/inputs/IMouseInput.hpp
  source/framework/inputs/InputStates.hpp
  source/ground_truth/GroundTruth.cpp
  source/ground_truth/GroundTruth.hpp
  source/ltc/LinearlyTransformedCosines.cpp
  source/ltc/LinearlyTransformedCosines.hpp
  source/point_light_cluster/PointLightCluster.cpp
  source/point_light_cluster/PointLightCluster.hpp
  source/framework/DebugOutput.hpp
  source/framework/DebugOutput.cpp
  source/interface/LightInterface.hpp
  source/interface/LightInterface.cpp
  source/interface/SceneInterface.hpp
  source/interface/SceneInterface.cpp
  source/interface/Interface.cpp
  source/interface/Interface.hpp
  source/interface/PointLightClusterInterface.cpp source/interface/PointLightClusterInterface.hpp source/interface/IInterfaceView.hpp)

add_executable(${PROJECT_NAME} ${AREALIGHTS_SOURCE_FILES} ${AREALIGHTS_DEPENDENCIES_SOURCE_FILES})

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

target_compile_definitions(${PROJECT_NAME} PUBLIC ${ADDITIONAL_DEFINITIONS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY} glfw)

set(PROJECT_COMPILE_FEATURES ${PROJECT_COMPILE_FEATURES} cxx_std_14 cxx_auto_type cxx_nullptr cxx_range_for)
target_compile_features(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_FEATURES})
