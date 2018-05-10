#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_arguments, returns_true_if_stream_has_enough_argument)
{
	{
		stringstream stringStream("0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(1));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, ' ', false, false));
	}
	{
		stringstream stringStream("0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, ' ', false, false));
	}
	{
		stringstream stringStream("0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(3));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, false, 0, false, true));
	}
}

TEST(skip_arguments, returns_false_if_stream_has_not_enough_argument)
{
	stringstream stringStream("0 1 2");
	Input input(stringStream);

	EXPECT_FALSE(input.SkipArguments<int>(4));
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, false, 0, false, true));
}

TEST(skip_arguments, skips_argument)
{
	stringstream stringStream("0 0.1 stringArgument");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArguments<string>(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, ' ', false, false));

	string lastArgument;
	EXPECT_TRUE(input.ReadArguments(true, lastArgument));
	EXPECT_EQ(lastArgument, "stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
}

TEST(skip_arguments, skips_end_of_line_by_default)
{
	stringstream stringStream("\n0 0.1 stringArgument");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArguments<string>(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 6, true, ' ', false, false));

	string lastArgument;
	EXPECT_TRUE(input.ReadArguments(true, lastArgument));
	EXPECT_EQ(lastArgument, "stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 21, false, 0, false, true));
}

TEST(skip_arguments, can_do_not_skip_end_of_line)
{
	{
		stringstream stringStream("\n0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipArguments<string>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '\n', true, false));
	}
	{
		stringstream stringStream("1ab\n0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<char>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, 'b', false, false));

		EXPECT_FALSE(input.SkipArguments<char>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, '\n', true, false));
	}
}
