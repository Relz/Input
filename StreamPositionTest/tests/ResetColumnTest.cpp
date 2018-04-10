#include "../../InputLibrary/StreamString/StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(reset_column, resets_column_to_minimum)
{
	TestHelper::TestStreamPosition(
		StreamPosition(1, 1),
		[](StreamPosition & streamPosition) { streamPosition.ResetColumn(); },
		1,
		StreamPosition::MIN_COLUMN);
	TestHelper::TestStreamPosition(
		StreamPosition(1, 2),
		[](StreamPosition & streamPosition) { streamPosition.ResetColumn(); },
		1,
		StreamPosition::MIN_COLUMN);
}
