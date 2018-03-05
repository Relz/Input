#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(decrease_column, decreases_column_by_one_by_default)
{
	TestStreamPosition(
		StreamPosition(2, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseColumn(); }, 1, 2);
}

TEST(decrease_column, decreases_column_by_value)
{
	TestStreamPosition(
		StreamPosition(2, 3), [](StreamPosition & streamPosition) { streamPosition.DecreaseColumn(2); }, 2, 1);
}

TEST(decrease_column, does_not_set_column_less_than_minimum)
{
	TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.DecreaseColumn(); }, 1, MIN_COLUMN);

	TestStreamPosition(
		StreamPosition(2, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseColumn(2); }, 1, MIN_COLUMN);
}
