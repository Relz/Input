#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_symbols, skips_until_characters_and_returns_true_if_any_character_was_reached)
{
	{
		stringstream stringStream("                       #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ '#' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilCharacters({ ' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("%\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));
	}
}

TEST(skip_until_symbols, skips_until_multiple_symbols)
{
	{
		stringstream stringStream("  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ 'a', '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, '\n', true, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilCharacters({ ' ', '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}
