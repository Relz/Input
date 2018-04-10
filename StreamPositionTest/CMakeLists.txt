PROJECT(StreamPositionTest)

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

ADD_SUBDIRECTORY(tests)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
if(CMAKE_COMPILER_IS_GNUCXX)
	INCLUDE(CodeCoverage)
	SETUP_TARGET_FOR_COVERAGE(StreamPositionTest_coverage StreamPositionTest coverage firefox)
endif()
