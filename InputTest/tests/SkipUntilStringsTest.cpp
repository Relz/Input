#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_strings, skips_until_strings_and_returns_true_if_any_symbol_was_reached)
{
	{
		stringstream is("                ##$a");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({ "#$" }, skippedString));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 18);
		EXPECT_EQ(skippedString, "                #");
		input.SkipArgument<char>();
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('$'));
	}
	{
		stringstream is("#");
		Input input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilStrings({ " " }, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
		EXPECT_EQ(skippedString, "#");
	}
	{
		stringstream is("%\n\na\nb\nc\nd\ne\nf\ng\nh\ni\nj\n#");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({ "#" }, skippedString));
		EXPECT_EQ(input.GetPosition().GetLine(), 13);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		input.SkipArguments<char>(2);
		EXPECT_TRUE(is.eof());
	}
}

TEST(skip_until_strings, skips_multiple_strings)
{
	{
		stringstream is("  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({ "aa", "\n" }, skippedString));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 6);
		EXPECT_EQ(skippedString, "  abc");
		input.SkipArguments<char>(2);
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
	}
	{
		stringstream is("#");
		Input input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilStrings({ " ", "\n" }, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
		EXPECT_EQ(skippedString, "#");
	}
}
