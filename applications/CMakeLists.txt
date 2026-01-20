function(voxl_add_app app_name)
  add_executable(${app_name})
  target_sources(${app_name} PRIVATE ${ARGN})
  target_link_libraries(${app_name} PRIVATE voxl)

  set_target_properties(${app_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/applications/${app_name}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/applications/${app_name}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/applications/${app_name}"
  )
endfunction()

voxl_add_app(sandbox ${CMAKE_CURRENT_SOURCE_DIR}/sandbox/main.cpp)