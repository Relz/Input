#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(decrease_line, decreases_line_by_one_by_default)
{
	TestStreamPosition(
		StreamPosition(2, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(); }, 1, 2);
}

TEST(decrease_line_, decreases_line_by_value)
{
	TestStreamPosition(
		StreamPosition(3, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(2); }, 1, 2);
}

TEST(decrease_line, does_not_set_line_less_than_minimum)
{
	TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(); }, MIN_LINE, 1);

	TestStreamPosition(
		StreamPosition(2, 2), [](StreamPosition & streamPosition) { streamPosition.DecreaseLine(2); }, MIN_LINE, 2);
}
