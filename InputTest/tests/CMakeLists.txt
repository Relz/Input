INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

SET(SOURCE_FILES
	ConstructorTest.cpp
	SkipArgumentTest.cpp
	SkipArgumentsTest.cpp
	SkipLineTest.cpp
	SkipLinesTest.cpp
	SkipCharactersTest.cpp
	SkipUntilCharactersTest.cpp
	ReadArgumentsTest.cpp
	ReadVectorTemplateTest.cpp
	ReadVectorTemplateWithRulesTest.cpp
	ReadVectorCharTest.cpp
	ReadVectorWithTrueCharTest.cpp
	ReadMatrixTemplateTest.cpp
	ScanTest.cpp
	SkipUntilStringsTest.cpp
	GetFileNameTest.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/InputLibrary)
ADD_EXECUTABLE(InputTest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(InputTest gtest gtest_main InputLibrary)
