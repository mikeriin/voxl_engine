function(voxl_add_app app_name)
  add_executable(${app_name})
  target_sources(${app_name} PRIVATE ${ARGN})
  target_link_libraries(${app_name} PRIVATE voxl)
endfunction()

voxl_add_app(sandbox ${CMAKE_CURRENT_SOURCE_DIR}/sandbox/main.cpp)