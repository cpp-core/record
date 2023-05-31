cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (timer VERSION 0.1 LANGUAGES C CXX ASM)

# Make including this file idempotent
#
if(TARGET timer OR TARGET timer::timer)
  return()
endif()

# Setup symlinks for this architecture
#
# set(TIMER_ARCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/words/${CMAKE_SYSTEM_PROCESSOR})
# if (EXISTS ${TIMER_ARCH_INCLUDE_DIR})
#   message("-- timer: Creating architecture symlink to ${CMAKE_SYSTEM_PROCESSOR}")
#   execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     ${TIMER_ARCH_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include/words/arch)
# endif()

# set(TIMER_ARCH_SRC_DIR ${CMAKE_SOURCE_DIR}/src/words/${CMAKE_SYSTEM_PROCESSOR})
# if (EXISTS ${TIMER_ARCH_SRC_DIR})
#   message("-- timer: Creating architecture symlink ${CMAKE_SYSTEM_PROCESSOR}")
#   execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     ${TIMER_ARCH_SRC_DIR}
#     ${CMAKE_SOURCE_DIR}/src/words/arch)
# endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation
  #
  option(TIMER_TEST "Generate the tests." ON)
  option(TIMER_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()

  # Setup compilation parameters
  #
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  
  add_compile_options(-Wall)
  add_compile_options(-g)
  add_compile_options(-O)

else()
  option(TIMER_TEST "Generate the tests." OFF)
  option(TIMER_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- timer: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- timer: Included from: ${CMAKE_SOURCE_DIR}")
message("-- timer: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- timer: test ${TIMER_TEST}")
message("-- timer: docs ${TIMER_DOCS}")

# Add our dependencies
#

# Build the library
#
set(SOURCES
  placeholder
  )

set(ASM_SOURCES
  )

set(FILES)
foreach(SOURCE ${SOURCES})
  list(APPEND FILES "src/core/timer/${SOURCE}.cpp")
endforeach()

foreach(SOURCE ${ASM_SOURCES})
  set(FILE "src/words/${CMAKE_SYSTEM_PROCESSOR}/${SOURCE}.s")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")
    list(APPEND FILES ${FILE})
  endif()
endforeach()

add_library(timer ${FILES})
add_library(timer::timer ALIAS timer)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(timer INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_include_directories(timer PUBLIC)
target_link_libraries(timer PUBLIC)

foreach(prog
    timer0
    )
  add_executable(${prog} src/tools/${prog}.cpp)
  target_link_libraries(${prog} timer::timer)
endforeach()

# Optionally configure the tests
#
if(TIMER_TEST)
 add_gtest()
 add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(TIMER_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
#install_config(timer)
