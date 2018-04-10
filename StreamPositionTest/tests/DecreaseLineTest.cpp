#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(decrease_line, decreases_line_by_one_by_default)
{
	TestHelper::TestStreamPosition(
		StreamPosition(2, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(); }, 1, 2);
}

TEST(decrease_line_, decreases_line_by_value)
{
	TestHelper::TestStreamPosition(
		StreamPosition(3, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(2); }, 1, 2);
}

TEST(decrease_line, does_not_set_line_less_than_minimum)
{
	TestHelper::TestStreamPosition(
		StreamPosition(),
		[](StreamPosition & streamPosition) { streamPosition.DecreaseLine(); },
		StreamPosition::MIN_LINE,
		1);

	TestHelper::TestStreamPosition(
		StreamPosition(2, 2),
		[](StreamPosition & streamPosition) { streamPosition.DecreaseLine(2); },
		StreamPosition::MIN_LINE,
		2);
}
