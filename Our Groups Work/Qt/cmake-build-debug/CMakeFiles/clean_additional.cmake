# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\dash_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\dash_autogen.dir\\ParseCache.txt"
  "dash_autogen"
  )
endif()
