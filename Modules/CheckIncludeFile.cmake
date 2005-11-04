#
# Check if the include file exists.
#
# CHECK_INCLUDE_FILE - macro which checks the include file exists.
# INCLUDE - name of include file
# VARIABLE - variable to return result
#
# an optional third argument is the CFlags to add to the compile line 
# or you can use CMAKE_REQUIRED_FLAGS
#
MACRO(CHECK_INCLUDE_FILE INCLUDE VARIABLE)
  IF("${VARIABLE}" MATCHES "^${VARIABLE}$")
    SET(MACRO_CHECK_INCLUDE_FILE_FLAGS ${CMAKE_REQUIRED_FLAGS})
    SET(CHECK_INCLUDE_FILE_VAR ${INCLUDE})
    CONFIGURE_FILE(${CMAKE_ROOT}/Modules/CheckIncludeFile.c.in
      ${CMAKE_BINARY_DIR}/CMakeTmp/CheckIncludeFile.c IMMEDIATE)
    MESSAGE(STATUS "Looking for ${INCLUDE}")
    IF(${ARGC} EQUAL 3)
      SET(CMAKE_C_FLAGS_SAVE ${CMAKE_C_FLAGS})
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARGV2}")
    ENDIF(${ARGC} EQUAL 3)

    TRY_COMPILE(${VARIABLE}
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/CMakeTmp/CheckIncludeFile.c
      CMAKE_FLAGS 
      -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_INCLUDE_FILE_FLAGS}
      OUTPUT_VARIABLE OUTPUT) 

    IF(${ARGC} EQUAL 3)
      SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_SAVE})
    ENDIF(${ARGC} EQUAL 3)

    IF(${VARIABLE})
      MESSAGE(STATUS "Looking for ${INCLUDE} - found")
      SET(${VARIABLE} 1 CACHE INTERNAL "Have include ${INCLUDE}")
      FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log 
        "Determining if the include file ${INCLUDE} "
        "exists passed with the following output:\n"
        "${OUTPUT}\n\n")
    ELSE(${VARIABLE})
      MESSAGE(STATUS "Looking for ${INCLUDE} - not found")
      SET(${VARIABLE} "" CACHE INTERNAL "Have include ${INCLUDE}")
      FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log 
        "Determining if the include file ${INCLUDE} "
        "exists failed with the following output:\n"
        "${OUTPUT}\n\n")
    ENDIF(${VARIABLE})
  ENDIF("${VARIABLE}" MATCHES "^${VARIABLE}$")
ENDMACRO(CHECK_INCLUDE_FILE)
