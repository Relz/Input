#include "../../StreamPosition/StreamPosition.h"
#include "gtest/gtest.h"

using namespace std;

TEST(not_equal_to_operator, returns_true_if_another_stream_position_has_different_line_or_column)
{
	EXPECT_TRUE(StreamPosition() != StreamPosition(1, 2));
	EXPECT_TRUE(StreamPosition() != StreamPosition(2, 1));
	EXPECT_TRUE(StreamPosition(3, 4) != StreamPosition(2, 1));
}

TEST(not_equal_to_operator, returns_false_if_another_stream_position_has_equal_line_and_column)
{
	EXPECT_FALSE(StreamPosition() != StreamPosition());
	EXPECT_FALSE(StreamPosition(2, 3) != StreamPosition(2, 3));
	EXPECT_FALSE(StreamPosition(-10, 0) != StreamPosition());
	EXPECT_FALSE(StreamPosition(-10, -1) != StreamPosition(-3, -5));
}
