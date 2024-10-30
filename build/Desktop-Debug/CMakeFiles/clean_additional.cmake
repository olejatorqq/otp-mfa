# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/otp-mfa_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/otp-mfa_autogen.dir/ParseCache.txt"
  "otp-mfa_autogen"
  )
endif()
