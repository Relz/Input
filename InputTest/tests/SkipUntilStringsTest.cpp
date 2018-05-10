#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_strings, skips_until_strings_and_returns_true_if_any_string_was_reached)
{
	{
		stringstream stringStream("                ##$a");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ "#$" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 18, true, '#', false, false));

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 19, true, '$', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilStrings({ " " }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ "#" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 1, true, '#', false, false));

		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 2, false, 0, false, true));
	}
}

TEST(skip_until_strings, skips_multiple_strings)
{
	{
		stringstream stringStream("  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ "aa", "\n" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, 'a', false, false));

		EXPECT_TRUE(input.SkipArguments<char>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, '\n', true, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilStrings({ " ", "\n" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}
