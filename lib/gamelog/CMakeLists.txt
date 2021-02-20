# Gamelog Library
set(LOG_LIB gamelog)

set(LOG_SOURCES
  src/gamelog.cpp
)

add_library(
  ${LOG_LIB}
  SHARED
  ${LOG_SOURCES}
)

target_link_libraries(
  ${LOG_LIB}
  PRIVATE project_options
          project_warnings
)

target_include_directories(
  ${LOG_LIB}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# System install
install(TARGETS ${LOG_LIB}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
