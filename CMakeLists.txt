cmake_minimum_required(VERSION 3.23)

project(algen_bf C CXX)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: 
      Debug Release RelWithDebInfo MinSizeRel. 
      Default is Release"
      FORCE)
endif()


include(cmake/fetch_content.cmake)

enable_testing()
include(GoogleTest)

add_subdirectory(exec)
add_subdirectory(lib)
add_subdirectory(test)
