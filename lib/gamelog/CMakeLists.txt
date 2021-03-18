# Gamelog Library
set(LOG_LIB gamelog)

set(LOG_SOURCES
  src/gamelog.cpp
)

set(LOG_HEADERS
  include/gamelog/gamelog.h
)

set(LOG_TESTS
  tests/test_main.cpp
  tests/test_gamelog.cpp
)

add_library(
  ${LOG_LIB}
  SHARED
  ${LOG_SOURCES}
)

set_target_properties(
  ${LOG_LIB}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION}
  PUBLIC_HEADER ${LOG_HEADERS}
)

target_link_libraries(
  ${LOG_LIB}
  PRIVATE project_options
          project_warnings
)

analyse_with_iwyu(${LOG_LIB})

target_include_directories(
  ${LOG_LIB}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# System install
install(TARGETS ${LOG_LIB}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LOG_LIB}
)

# Unit Tests
if (ENABLE_TESTING)
  # only build when modern target exists
  if (TARGET GTest::GTest)
    add_executable(
      ${LOG_LIB}-test
      ${LOG_TESTS}
    )
    target_link_libraries(
      ${LOG_LIB}-test
      PRIVATE
        ${LOG_LIB}
        GTest::GTest
        GTest::Main
        project_options
        project_warnings
    )

    add_test(
      ${LOG_LIB}-test
      ${LOG_LIB}-test
    )

    install(
      TARGETS ${LOG_LIB}-test
      RUNTIME DESTINATION build
    )
  endif()
endif()
