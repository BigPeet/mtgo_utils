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

analyse_with_iwyu(${FILE_PARSER})

# System install
install(TARGETS ${FILE_PARSER}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


# Find Bash for potential bash scripts
find_program(BASH_PROGRAM bash)

# Add some bash tests for the executable
if(ENABLE_TESTING AND BASH_PROGRAM)
  add_test(${FILE_PARSER}_test_no_arguments
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_no_arguments.sh
  )
  add_test(${FILE_PARSER}_test_nonexisting_file
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_nonexisting_file.sh
  )
  add_test(${FILE_PARSER}_test_no_matchgamelog_file
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_no_matchgamelog_file.sh
  )
  add_test(${FILE_PARSER}_test_matchgamelog_01
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_matchgamelog_01.sh
  )
  add_test(${FILE_PARSER}_test_matchgamelog_02
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_matchgamelog_02.sh
  )
  add_test(${FILE_PARSER}_test_matchgamelog_03
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_matchgamelog_03.sh
  )
  add_test(${FILE_PARSER}_test_matchgamelog_04
           ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_matchgamelog_04.sh
  )
endif()
