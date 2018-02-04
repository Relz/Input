#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(reset_column, resets_column_to_minimum)
{
	TestStreamPosition(
		StreamPosition(1, 1), [](StreamPosition& streamPosition) { streamPosition.ResetColumn(); }, 1, MIN_COLUMN);
	TestStreamPosition(
		StreamPosition(1, 2), [](StreamPosition& streamPosition) { streamPosition.ResetColumn(); }, 1, MIN_COLUMN);
}
