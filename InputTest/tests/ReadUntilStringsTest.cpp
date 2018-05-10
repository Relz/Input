#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_until_strings, reads_until_strings_to_passed_parameter_and_returns_true_if_any_string_was_reached)
{
	{
		stringstream stringStream("                ##$a");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilStrings({ "#$" }, readString));
		EXPECT_EQ(readString, "                #");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 18, true, '#', false, false));
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 19, true, '$', false, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		string readString;
		EXPECT_FALSE(input.ReadUntilStrings({ " " }, readString));
		EXPECT_EQ(readString, "#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n#");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilStrings({ "#" }, readString));
		EXPECT_EQ(readString, "%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n");
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 1, true, '#', false, false));
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_TRUE(TestHelper::CheckState(input, 13, 2, false, 0, false, true));
	}
}

TEST(read_until_strings, reads_multiple_strings)
{
	{
		stringstream stringStream("  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(stringStream);

		string readString;
		EXPECT_TRUE(input.ReadUntilStrings({ "aa", "\n" }, readString));
		EXPECT_EQ(readString, "  abc");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, 'a', false, false));
		EXPECT_TRUE(input.SkipArguments<char>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, true, false));
	}
	{
		stringstream stringStream("#");
		Input input(stringStream);

		string readString;
		EXPECT_FALSE(input.ReadUntilStrings({ " ", "\n" }, readString));
		EXPECT_EQ(readString, "#");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
}

TEST(read_until_strings, overwrites_read_string_by_default)
{
	stringstream stringStream("another string##");
	Input input(stringStream);

	string readString = "some string";
	EXPECT_TRUE(input.ReadUntilStrings({ "##" }, readString));
	EXPECT_EQ(readString, "another string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, '#', false, false));
}

TEST(read_until_strings, can_append_to_read_string)
{
	stringstream stringStream("another string##");
	Input input(stringStream);

	string readString = "some string";
	EXPECT_TRUE(input.ReadUntilStrings({ "##" }, readString, true));
	EXPECT_EQ(readString, "some stringanother string");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 15, true, '#', false, false));
}
