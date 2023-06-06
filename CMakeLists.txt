cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (record VERSION 0.1 LANGUAGES C CXX ASM)

# Make including this file idempotent
#
if(TARGET record OR TARGET record::record)
  return()
endif()

# Setup symlinks for this architecture
#
# set(RECORD_ARCH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/words/${CMAKE_SYSTEM_PROCESSOR})
# if (EXISTS ${RECORD_ARCH_INCLUDE_DIR})
#   message("-- record: Creating architecture symlink to ${CMAKE_SYSTEM_PROCESSOR}")
#   execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     ${RECORD_ARCH_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include/words/arch)
# endif()

# set(RECORD_ARCH_SRC_DIR ${CMAKE_SOURCE_DIR}/src/words/${CMAKE_SYSTEM_PROCESSOR})
# if (EXISTS ${RECORD_ARCH_SRC_DIR})
#   message("-- record: Creating architecture symlink ${CMAKE_SYSTEM_PROCESSOR}")
#   execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     ${RECORD_ARCH_SRC_DIR}
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
  option(RECORD_TEST "Generate the tests." ON)
  option(RECORD_DOCS "Generate the docs." OFF)

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
  option(RECORD_TEST "Generate the tests." OFF)
  option(RECORD_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- record: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- record: Included from: ${CMAKE_SOURCE_DIR}")
message("-- record: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- record: test ${RECORD_TEST}")
message("-- record: docs ${RECORD_DOCS}")

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
  list(APPEND FILES "src/core/record/${SOURCE}.cpp")
endforeach()

foreach(SOURCE ${ASM_SOURCES})
  set(FILE "src/words/${CMAKE_SYSTEM_PROCESSOR}/${SOURCE}.s")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")
    list(APPEND FILES ${FILE})
  endif()
endforeach()

add_library(record ${FILES})
add_library(record::record ALIAS record)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(record INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_include_directories(record PUBLIC)
target_link_libraries(record PUBLIC)

foreach(prog
    record0
    record1
    )
  add_executable(${prog} src/tools/${prog}.cpp)
  target_link_libraries(${prog} record::record)
endforeach()

# Optionally configure the tests
#
if(RECORD_TEST)
 add_gtest()
 add_subdirectory(test)
endif()

# Optionally configure the documentation
#
if(RECORD_DOCS)
  add_subdirectory(docs)
endif()

# Installation
#
#install_config(record)
