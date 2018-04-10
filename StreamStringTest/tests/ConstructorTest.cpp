#include "../../StreamString/StreamString.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(empty_constructor, sets_default_string_and_position)
{
	std::string str = "test string";
	EXPECT_TRUE(TestHelper::TestStreamString(StreamString(), std::string(), StreamPosition()));
	EXPECT_TRUE(TestHelper::TestStreamString(StreamString(str), str, StreamPosition()));
}

TEST(set_constructor, sets_string_and_position_to_specified_values)
{
	std::string str = "test string";
	StreamPosition position = StreamPosition(2, 3);
	EXPECT_TRUE(TestHelper::TestStreamString(StreamString(str, position), str, position));
}
