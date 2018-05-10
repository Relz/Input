#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_line, returns_false_if_end_of_line_was_not_reached)
{
	{
		stringstream stringStream("string");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_FALSE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 7, false, 0, false, true));
	}
	{
		stringstream stringStream("string\n");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_FALSE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
}

TEST(skip_line, skips_line_lf)
{
	{
		stringstream stringStream("0 0.1 stringArgument \n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '\n', true, false));
	}
}

TEST(skip_line, skips_line_cr)
{
	{
		stringstream stringStream("0 0.1 stringArgument \r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\r\r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '\r', true, false));
	}
}

TEST(skip_line, skips_line_crlf)
{
	{
		stringstream stringStream("0 0.1 stringArgument \r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\r\n\r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, '#', false, false));
	}
}
