add_library(voxl STATIC)

file(GLOB_RECURSE VOXL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(voxl PRIVATE ${VOXL_SOURCES})

target_include_directories(voxl 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Deps externes via vcpkg:
find_package(SDL3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Deps externes non prises en charge par vcpkg
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build Noise Tool" FORCE)
add_subdirectory(
  ${CMAKE_SOURCE_DIR}/external/FastNoise2
  ${CMAKE_BINARY_DIR}/_deps/FastNoise2
)


target_link_libraries(voxl PUBLIC
  SDL3::SDL3
  EnTT::EnTT
  glad::glad
  glm::glm
  FastNoise
  opengl32
)
