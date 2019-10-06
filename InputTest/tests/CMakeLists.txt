INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

SET(SOURCE_FILES
	ConstructorTest.cpp
	EmplaceBackElementTest.cpp
	EmplaceBackVectorIfNotEmptyTest.cpp
	GetCharPositionTest.cpp
	GetFileNameTest.cpp
	GetSizeTest.cpp
	IsEndOfStreamTest.cpp
	IsEndOfLineTest.cpp
	SkipLineTest.cpp
	SkipLinesTest.cpp
	SkipArgumentTest.cpp
	SkipArgumentsTest.cpp
	SkipCharactersTest.cpp
	SkipUntilCharactersTest.cpp
	SkipUntilStringsTest.cpp
	ReadLineTest.cpp
	ReadArgumentsTest.cpp
	ReadUntilCharactersTest.cpp
	ReadUntilStringsTest.cpp
	ReadVectorTest.cpp
	ReadMatrixTest.cpp
	ScanTest.cpp
	TestHelper.h)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/InputLibrary)
ADD_EXECUTABLE(InputTest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(InputTest gtest gtest_main InputLibrary)
