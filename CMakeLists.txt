cmake_minimum_required (VERSION 3.5)

include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.16.3.tar.gz"
  SHA1 "9a9ef7e83c822f95de7b32f44114fa1b1edda5d0"
)
project(4K_displaywall_bench)

if(MSVC)
  hunter_add_package(glfw)
  find_package(glfw3 REQUIRED)

  hunter_add_package(glm)
  find_package(glm REQUIRED)
  set(dep_libraries glfw glm)
else()
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(LibDL REQUIRED)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  # glm is header-only
  set(dep_includes ${LIBDL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})
  set(dep_libraries ${LIBDL_LIBRARY} ${GLFW_LIBRARIES})
  set(dep_flags ${GLFW_CFLAGS_OTHER})
endif()

add_executable (4K_displaywall_bench main.cpp Scene.cpp png/lodepng.cpp glad/glad.c)
target_include_directories(4K_displaywall_bench PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" ${dep_includes})
target_compile_definitions(4K_displaywall_bench PRIVATE _USE_MATH_DEFINES)
target_compile_options(4K_displaywall_bench PRIVATE ${dep_flags})
target_link_libraries(4K_displaywall_bench PRIVATE ${dep_libraries})
