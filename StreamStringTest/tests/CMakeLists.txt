INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

SET(SOURCE_FILES
		ConstructorTest.cpp)

ADD_EXECUTABLE(StreamStringTest ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(StreamStringTest gtest gtest_main)
TARGET_LINK_LIBRARIES(StreamStringTest StreamString)
