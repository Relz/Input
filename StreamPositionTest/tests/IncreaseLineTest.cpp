#include "../../StreamPosition/StreamPosition.h"
#include "TestHelper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(increase_line, increases_line_by_one_by_default)
{
	TestHelper::TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.IncreaseLine(); }, 2, 1);
}

TEST(increase_line, increases_line_by_value)
{
	TestHelper::TestStreamPosition(
		StreamPosition(), [](StreamPosition & streamPosition) { streamPosition.IncreaseLine(2); }, 3, 1);
}
