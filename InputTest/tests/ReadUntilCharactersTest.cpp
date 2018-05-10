#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_until_characters, reads_until_characters_and_returns_true_if_any_character_was_reached)
{
	{
		stringstream stringStream("                       #");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ '#' }, readString));
		EXPECT_EQ(readString, "                       ");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 24, true, '#', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		string readString;
		EXPECT_FALSE(input.ReadUntilCharacters({ ' ' }, readString));
		EXPECT_EQ(readString, "#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("%\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ '\n' }, readString));
		EXPECT_EQ(readString, "%");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));
	}
}

TEST(read_until_characters, reads_until_multiple_characters)
{
	{
		stringstream stringStream("  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilCharacters({ 'a', '\n' }, readString));
		EXPECT_EQ(readString, "  ");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, '\n', true, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		string readString;
		EXPECT_FALSE(input.ReadUntilCharacters({ ' ', '\n' }, readString));
		EXPECT_EQ(readString, "#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(read_until_characters, overwrites_read_string_by_default)
{
	stringstream stringStream("another string#");
	Input input(stringStream);

	string readString = "some string";
	EXPECT_TRUE(input.ReadUntilCharacters({ '#' }, readString));
	EXPECT_EQ(readString, "another string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, '#', false, false));
}

TEST(read_until_characters, can_append_to_read_string)
{
	stringstream stringStream("another string#");
	Input input(stringStream);

	string readString = "some string";
	EXPECT_TRUE(input.ReadUntilCharacters({ '#' }, readString, true));
	EXPECT_EQ(readString, "some stringanother string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, '#', false, false));
}
