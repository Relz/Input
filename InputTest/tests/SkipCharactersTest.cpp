#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_symbols, skips_single_symbol_and_returns_true_if_any_symbol_was_skipped)
{
	{
		stringstream stringStream("                       #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ ' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ ' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 12, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ '#' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(skip_symbols, skips_multiple_symbols)
{
	{
		stringstream stringStream("  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ ' ', '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 12, 4, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ ' ', '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '#', false, false));
	}
}
