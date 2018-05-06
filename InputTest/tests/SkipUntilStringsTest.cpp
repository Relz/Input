#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_strings, skips_until_strings_and_returns_true_if_any_string_was_reached)
{
	{
		wstringstream stringStream(L"                ##$a");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ L"#$" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 18, true, L'#', false, false));

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 19, true, L'$', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilStrings({ L" " }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ L"#" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 1, true, L'#', false, false));

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 2, false, 0, false, true));
	}
}

TEST(skip_until_strings, skips_multiple_strings)
{
	{
		wstringstream stringStream(L"  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipUntilStrings({ L"aa", L"\n" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L'a', false, false));

		EXPECT_TRUE(input.SkipArguments<wchar_t>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipUntilStrings({ L" ", L"\n" }));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}
