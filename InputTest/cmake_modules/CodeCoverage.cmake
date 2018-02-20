FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH lcov )
FIND_PROGRAM( GENHTML_PATH genhtml )
FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

IF(NOT GCOV_PATH)
    MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF() # NOT GCOV_PATH

IF(NOT LCOV_PATH)
    FIND_PROGRAM( LCOV_PATH lcov.bat )
ENDIF() # NOT LCOV_PATH

IF(NOT GENHTML_PATH)
    FIND_PROGRAM( GENHTML_PATH genhtml.bat )
ENDIF() # NOT GENHTML_PATH

IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    # Clang version 3.0.0 and greater now supports gcov as well.
    MESSAGE(WARNING "Compiler is not GNU gcc! Clang Version 3.0.0 and greater supports gcov as well, but older versions don't.")

    IF(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
    ENDIF()
ENDIF() # NOT CMAKE_COMPILER_IS_GNUCXX

SET(CMAKE_CXX_FLAGS_COVERAGE
        "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
        CACHE STRING "Flags used by the C++ compiler during coverage builds."
        FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
        "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
        CACHE STRING "Flags used by the C compiler during coverage builds."
        FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
        ""
        CACHE STRING "Flags used for linking binaries during coverage builds."
        FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
        ""
        CACHE STRING "Flags used by the shared libraries linker during coverage builds."
        FORCE )
MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_COVERAGE
        CMAKE_C_FLAGS_COVERAGE
        CMAKE_EXE_LINKER_FLAGS_COVERAGE
        CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

IF ( NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage"))
    MESSAGE( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
ENDIF() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"


# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#						MUST return ZERO always, even on errors.
#						If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
# Param _browsercommand  Browser command to view coverage html report
FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname _browsercommand)

    IF(NOT LCOV_PATH)
        MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
    ENDIF() # NOT LCOV_PATH

    IF(NOT GENHTML_PATH)
        MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
    ENDIF() # NOT GENHTML_PATH

    # Setup target
    ADD_CUSTOM_TARGET(${_targetname}

            # Cleanup lcov
            ${LCOV_PATH} --directory . --zerocounters

            # Run tests
            COMMAND ${_testrunner}

            # Capturing lcov counters and generating report
            COMMAND ${LCOV_PATH} --rc lcov_branch_coverage=1 --directory . --capture --output-file ${_outputname}.info
            COMMAND ${LCOV_PATH} --rc lcov_branch_coverage=1 --remove ${_outputname}.info 'build/*' 'tests/*' '/usr/*' '${gtest_SOURCE_DIR}/*' '*/**/StreamPosition/*' --output-file ${_outputname}.info.cleaned
            COMMAND ${GENHTML_PATH} --rc lcov_branch_coverage=1 -o ${_outputname} ${_outputname}.info.cleaned
            COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.info ${_outputname}.info.cleaned

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
            )

    # View html report in specified browser
    ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
            COMMAND ${_browsercommand} ${CMAKE_BINARY_DIR}/${_outputname}/index.html;
            COMMENT "Opening ${CMAKE_BINARY_DIR}/${_outputname}/index.html in your browser to view the coverage report."
            )

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE
