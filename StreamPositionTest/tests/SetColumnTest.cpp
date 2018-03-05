#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(set_column, sets_column)
{
	TestStreamPosition(StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.SetColumn(10); }, 1, 10);
	TestStreamPosition(
		StreamPosition(1, 2), [](StreamPosition & streamPosition) { streamPosition.SetColumn(11); }, 1, 1);
}

TEST(set_column, does_not_set_column_less_than_minimum)
{
	TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.SetColumn(0); }, 1, MIN_COLUMN);
	TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.SetColumn(-1); }, 1, MIN_COLUMN);
}
