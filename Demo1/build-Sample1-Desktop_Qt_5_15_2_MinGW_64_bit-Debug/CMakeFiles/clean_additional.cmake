# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Sample1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Sample1_autogen.dir\\ParseCache.txt"
  "Sample1_autogen"
  )
endif()
