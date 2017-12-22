INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

SET(SOURCE_FILES
        ConstructorTest.cpp
		SkipArgumentTest.cpp
		SkipArgumentsTest.cpp
        SkipLineTest.cpp
        SkipLinesTest.cpp
		SkipSymbolsTest.cpp
		SkipUntilSymbolsTest.cpp
        ReadArgumentsTest.cpp
        ReadVectorTemplateTest.cpp
        ReadVectorTemplateWithRulesTest.cpp
        ReadVectorWCharTest.cpp
        ReadVectorWithTrueCharTest.cpp
        ReadMatrixTemplateTest.cpp
		VectorPushTest.cpp
		ScanTest.cpp
        SkipUntilStringsTest.cpp)

ADD_EXECUTABLE(InputTest ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(InputTest gtest gtest_main)
TARGET_LINK_LIBRARIES(InputTest Input)
