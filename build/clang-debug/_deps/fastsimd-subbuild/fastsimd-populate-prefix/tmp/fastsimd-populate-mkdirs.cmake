# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-src")
  file(MAKE_DIRECTORY "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-src")
endif()
file(MAKE_DIRECTORY
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-build"
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix"
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/tmp"
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/src/fastsimd-populate-stamp"
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/src"
  "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/src/fastsimd-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/src/fastsimd-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/Projects/voxl_engine/build/clang-debug/_deps/fastsimd-subbuild/fastsimd-populate-prefix/src/fastsimd-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
