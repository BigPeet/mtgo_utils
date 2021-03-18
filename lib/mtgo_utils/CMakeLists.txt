# mtgo_utils Library
set(LIB_NAME mtgo_utils)

set(LIB_SOURCES
  src/gamelog.cpp
)

set(LIB_HEADERS
  include/gamelog/gamelog.h
)

set(LIB_TESTS
  tests/test_main.cpp
  tests/test_gamelog.cpp
)

add_library(
  ${LIB_NAME}
  SHARED
  ${LIB_SOURCES}
)

set_target_properties(
  ${LIB_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION}
  PUBLIC_HEADER ${LIB_HEADERS}
)

target_link_libraries(
  ${LIB_NAME}
  PRIVATE project_options
          project_warnings
)

analyse_with_iwyu(${LIB_NAME})

target_include_directories(
  ${LIB_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# System install
install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}
)

# Unit Tests
if (ENABLE_TESTING)
  add_executable(
    ${LIB_NAME}-test
    ${LIB_TESTS}
  )
  target_link_libraries(
    ${LIB_NAME}-test
    PRIVATE
      ${LIB_NAME}
      gtest
      gtest_main
      project_options
      project_warnings
  )

  add_test(
    NAME ${LIB_NAME}-test
    COMMAND ${LIB_NAME}-test
  )

  install(
    TARGETS ${LIB_NAME}-test
    RUNTIME DESTINATION build
  )
endif()