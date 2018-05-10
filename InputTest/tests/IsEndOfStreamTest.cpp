#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(is_end_of_stream, returns_true_if_end_of_stream)
{
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '#', false, false));

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("#\n");
		Input input(stringStream);

		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '#', false, false));

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
}
