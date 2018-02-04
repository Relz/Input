#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(set_line, sets_line)
{
	TestStreamPosition(StreamPosition(), [](StreamPosition& streamPosition) { streamPosition.SetLine(10); }, 1, 10);
	TestStreamPosition(StreamPosition(1, 2), [](StreamPosition& streamPosition) { streamPosition.SetLine(11); }, 1, 1);
}

TEST(set_line, does_not_set_line_less_than_minimum)
{
	TestStreamPosition(
		StreamPosition(), [](StreamPosition& streamPosition) { streamPosition.SetLine(0); }, MIN_LINE, 1);
	TestStreamPosition(
		StreamPosition(), [](StreamPosition& streamPosition) { streamPosition.SetLine(-1); }, MIN_LINE, 1);
}
