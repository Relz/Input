#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_symbols, skips_until_characters_and_returns_true_if_any_character_was_reached)
{
	{
		wstringstream stringStream(L"                       #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ L'#' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilCharacters({ L' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"%\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
	}
}

TEST(skip_until_symbols, skips_until_multiple_symbols)
{
	{
		wstringstream stringStream(L"  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilCharacters({ L'a', L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilCharacters({ L' ', L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}
