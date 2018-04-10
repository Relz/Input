#include "../../InputLibrary/StreamString/StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <climits>

using namespace std;

TEST(empty_constructor, sets_line_and_column_to_one)
{
	EXPECT_TRUE(TestHelper::TestStreamPosition(StreamPosition(), TestHelper::EmptyFunction, 1, 1));
}

TEST(set_constructor, sets_line_and_column_to_specified_values_but_not_less_than_min_values)
{
	EXPECT_TRUE(TestHelper::TestStreamPosition(
		StreamPosition(-1, 0), TestHelper::EmptyFunction, StreamPosition::MIN_LINE, StreamPosition::MIN_COLUMN));
	EXPECT_TRUE(TestHelper::TestStreamPosition(
		StreamPosition(0, -1), TestHelper::EmptyFunction, StreamPosition::MIN_LINE, StreamPosition::MIN_COLUMN));
	EXPECT_TRUE(TestHelper::TestStreamPosition(StreamPosition(2, 3), TestHelper::EmptyFunction, 2, 3));
}

TEST(set_constructor, sets_line_and_column_to_specified_values_but_not_more_than_max_long)
{
	EXPECT_TRUE(TestHelper::TestStreamPosition(
		StreamPosition(LONG_MAX + 1, LONG_MAX + 1),
		TestHelper::EmptyFunction,
		StreamPosition::MIN_LINE,
		StreamPosition::MIN_COLUMN));
	EXPECT_TRUE(TestHelper::TestStreamPosition(
		StreamPosition(LONG_MAX, LONG_MAX), TestHelper::EmptyFunction, LONG_MAX, LONG_MAX));
	EXPECT_TRUE(TestHelper::TestStreamPosition(
		StreamPosition(LONG_MAX - 1, LONG_MAX - 1), TestHelper::EmptyFunction, LONG_MAX - 1, LONG_MAX - 1));
}
