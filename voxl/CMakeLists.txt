add_library(voxl STATIC)

target_sources(voxl PRIVATE
  src/voxl.cpp
)

target_include_directories(voxl PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Deps externes via vcpkg:
find_package(SDL3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

target_link_libraries(voxl PUBLIC
  SDL3::SDL3
  EnTT::EnTT
  glad::glad
  opengl32
)
