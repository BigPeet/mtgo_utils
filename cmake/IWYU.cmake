# Setup IWYU
find_program(IWYU_PATH NAMES include-what-you-use iwyu REQUIRED)
if(NOT ${IWYU_PATH} STREQUAL "IWYU_PATH-NOTFOUND")
  set(IWYU_FOUND TRUE)
else()
  set(IWYU_FOUND FALSE)
endif()

# Disable IWYU by default.
if(NOT ENABLE_IWYU)
  set(ENABLE_IWYU OFF)
endif()

if(IWYU_FOUND AND ENABLE_IWYU)
  message(STATUS "Building with IWYU.")
  set(IWYU_ACTIVE TRUE)
else()
  message(STATUS "Building without IWYU.")
  set(IWYU_ACTIVE FALSE)
endif()

# Use IWYU on the given target
function(analyse_with_iwyu target_name)
  if(IWYU_ACTIVE)
    set_target_properties(${target_name}
      PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH}
    )
  endif()
endfunction()
