# File Parser Executable
set(FILE_PARSER file_parser)

set(FILE_PARSER_SOURCES
  src/file_parser.cpp
)

add_executable(${FILE_PARSER} ${FILE_PARSER_SOURCES})
target_link_libraries(
  ${FILE_PARSER}
  PRIVATE project_options
          project_warnings
          gamelog
)

# Local install
install(TARGETS ${FILE_PARSER}
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/install/bin
)

# System install
install(TARGETS ${FILE_PARSER}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
