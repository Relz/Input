#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_argument, returns_false_if_stream_has_not_argument)
{
	stringstream stringStream("0");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArgument<char>());
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));

	EXPECT_FALSE(input.SkipArgument<char>());
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(skip_argument, skips_argument)
{
	{
		stringstream stringStream("0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, ' ', false, false));

		double number;
		EXPECT_TRUE(input.ReadArguments(true, number));
		EXPECT_EQ(number, 0.1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, ' ', false, false));
	}
	{
		stringstream stringStream("0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, ' ', false, false));
	}
	{
		stringstream stringStream("0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, ' ', false, false));

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, ' ', false, false));

		string lastArgument;
		EXPECT_TRUE(input.ReadArguments(true, lastArgument));
		EXPECT_EQ(lastArgument, "stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
	}
}

TEST(skip_argument, skips_end_of_line_by_default)
{
	{
		stringstream stringStream("\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '3', false, false));
	}
	{
		stringstream stringStream("   \n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 2, true, ' ', false, false));
	}
	{
		stringstream stringStream("\n3\n0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 2, true, '\n', true, false));

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 4, true, ' ', false, false));
	}
}

TEST(skip_argument, can_do_not_skip_end_of_line)
{
	{
		stringstream stringStream("\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipArgument<char>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '\n', true, false));
	}
	{
		stringstream stringStream("1\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<char>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));

		EXPECT_FALSE(input.SkipArgument<char>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));
	}
	{
		stringstream stringStream("1\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));

		EXPECT_FALSE(input.SkipArgument<string>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));
	}
}
