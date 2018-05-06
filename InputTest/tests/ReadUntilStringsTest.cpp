#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_until_strings, reads_until_strings_to_passed_parameter_and_returns_true_if_any_string_was_reached)
{
	{
		wstringstream stringStream(L"                ##$a");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilStrings({ L"#$" }, readString));
		EXPECT_EQ(readString, L"                #");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 18, true, L'#', false, false));
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 19, true, L'$', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		wstring readString;
		EXPECT_FALSE(input.ReadUntilStrings({ L" " }, readString));
		EXPECT_EQ(readString, L"#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n#");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilStrings({ L"#" }, readString));
		EXPECT_EQ(readString, L"%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n");
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 1, true, L'#', false, false));
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 2, false, 0, false, true));
	}
}

TEST(read_until_strings, reads_multiple_strings)
{
	{
		wstringstream stringStream(L"  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilStrings({ L"aa", L"\n" }, readString));
		EXPECT_EQ(readString, L"  abc");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L'a', false, false));
		EXPECT_TRUE(input.SkipArguments<wchar_t>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, true, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		wstring readString;
		EXPECT_FALSE(input.ReadUntilStrings({ L" ", L"\n" }, readString));
		EXPECT_EQ(readString, L"#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(read_until_strings, overwrites_read_string_by_default)
{
	wstringstream stringStream(L"another string##");
	Input input(stringStream);

	wstring readString = L"some string";
	EXPECT_TRUE(input.ReadUntilStrings({ L"##" }, readString));
	EXPECT_EQ(readString, L"another string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, L'#', false, false));
}

TEST(read_until_strings, can_append_to_read_string)
{
	wstringstream stringStream(L"another string##");
	Input input(stringStream);

	wstring readString = L"some string";
	EXPECT_TRUE(input.ReadUntilStrings({ L"##" }, readString, true));
	EXPECT_EQ(readString, L"some stringanother string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, L'#', false, false));
}
