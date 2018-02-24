#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(skip_until_symbols, skips_until_symbols_and_returns_true_if_any_symbol_was_reached)
{
	{
		stringstream is("                       #");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilCharacters({'#'}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 24);
		EXPECT_EQ(skippedString, "                       ");
	}
	{
		stringstream is("#");
		Input input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilCharacters({' '}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
		EXPECT_EQ(skippedString, "#");
	}
	{
		stringstream is("%\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilCharacters({'\n'}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
		EXPECT_EQ(skippedString, "%");
	}
}

TEST(skip_until_symbols, skips_multiple_symbols)
{
	{
		stringstream is("  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilCharacters({'a', '\n'}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 3);
		EXPECT_EQ(skippedString, "  ");
	}
	{
		stringstream is("#");
		Input input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilCharacters({' ', '\n'}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
		EXPECT_EQ(skippedString, "#");
	}
}
