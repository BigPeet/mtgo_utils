cmake_minimum_required(VERSION 3.15)

project(mtgo_utils VERSION 0.1.1)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# for correct library locations across platforms
include(GNUInstallDirs)

if(WIN32)
  # Set this option so Windows will export all symbols from a .dll.
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Setup IWYU
include(cmake/IWYU.cmake)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Enable Address and UB sanitizers for debug build.")
  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR TRUE)
endif()
enable_sanitizers(project_options)

# Enable / Disable Testing
if(NOT ENABLE_TESTING)
  set(ENABLE_TESTING OFF)
endif()
if(ENABLE_TESTING)
  message(STATUS "Enable testing.")
  enable_testing()
endif()

# Add source code directories
add_subdirectory(lib)
add_subdirectory(src)
