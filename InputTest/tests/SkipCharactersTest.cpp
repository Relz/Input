#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_symbols, skips_single_symbol_and_returns_true_if_any_symbol_was_skipped)
{
	{
		wstringstream stringStream(L"                       #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ L' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ L' ' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 12, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ '\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ L'#' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(skip_symbols, skips_multiple_symbols)
{
	{
		wstringstream stringStream(L"  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipCharacters({ L' ', L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 12, 4, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipCharacters({ L' ', L'\n' }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'#', false, false));
	}
}
