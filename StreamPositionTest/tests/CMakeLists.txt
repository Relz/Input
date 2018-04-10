INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

SET(SOURCE_FILES
		ConstructorTest.cpp
		SetLineTest.cpp
		SetColumnTest.cpp
		IncreaseLineTest.cpp
		DecreaseLineTest.cpp
		IncreaseColumnTest.cpp
		DecreaseColumnTest.cpp
		TestHelper.h
		ResetColumnTest.cpp
		EqualToOperatorTest.cpp
		NotEqualToOperatorTest.cpp)

ADD_EXECUTABLE(StreamPositionTest ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(StreamPositionTest gtest gtest_main)
TARGET_LINK_LIBRARIES(StreamPositionTest StreamPosition)
