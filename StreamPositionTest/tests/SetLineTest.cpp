#include "../../InputLibrary/StreamString/StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(set_line, sets_line)
{
	TestHelper::TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.SetLine(10); }, 1, 10);
	TestHelper::TestStreamPosition(
		StreamPosition(1, 2), [](StreamPosition & streamPosition) { streamPosition.SetLine(11); }, 1, 1);
}

TEST(set_line, does_not_set_line_less_than_minimum)
{
	TestHelper::TestStreamPosition(
		StreamPosition(),
		[](StreamPosition & streamPosition) { streamPosition.SetLine(0); },
		StreamPosition::MIN_LINE,
		1);
	TestHelper::TestStreamPosition(
		StreamPosition(),
		[](StreamPosition & streamPosition) { streamPosition.SetLine(-1); },
		StreamPosition::MIN_LINE,
		1);
}
