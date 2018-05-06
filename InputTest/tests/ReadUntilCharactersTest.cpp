#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_until_characters, reads_until_characters_and_returns_true_if_any_character_was_reached)
{
	{
		wstringstream stringStream(L"                       #");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ L'#' }, readString));
		EXPECT_EQ(readString, L"                       ");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		wstring readString;
		EXPECT_FALSE(input.ReadUntilCharacters({ L' ' }, readString));
		EXPECT_EQ(readString, L"#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"%\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ L'\n' }, readString));
		EXPECT_EQ(readString, L"%");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
	}
}

TEST(read_until_characters, reads_until_multiple_characters)
{
	{
		wstringstream stringStream(L"  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		wstring readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ L'a', L'\n' }, readString));
		EXPECT_EQ(readString, L"  ");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"#");
		Input input(stringStream);

		wstring readString;
		EXPECT_FALSE(input.ReadUntilCharacters({ L' ', L'\n' }, readString));
		EXPECT_EQ(readString, L"#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(read_until_characters, overwrites_read_string_by_default)
{
	wstringstream stringStream(L"another string#");
	Input input(stringStream);

	wstring readString = L"some string";
	EXPECT_TRUE(input.ReadUntilCharacters({ L'#' }, readString));
	EXPECT_EQ(readString, L"another string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, L'#', false, false));
}

TEST(read_until_characters, can_append_to_read_string)
{
	wstringstream stringStream(L"another string#");
	Input input(stringStream);

	wstring readString = L"some string";
	EXPECT_TRUE(input.ReadUntilCharacters({ L'#' }, readString, true));
	EXPECT_EQ(readString, L"some stringanother string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, L'#', false, false));
}
