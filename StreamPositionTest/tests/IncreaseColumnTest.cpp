#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(increase_column, increases_column_by_one_by_default)
{
	TestHelper::TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.IncreaseColumn(); }, 1, 2);
}

TEST(increase_column, increases_column_by_value)
{
	TestHelper::TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.IncreaseColumn(2); }, 1, 3);
}
