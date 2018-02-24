#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

char const SPACE_SYMBOL = ' ';
char const ENDL_SYMBOL = '\n';

TEST(skip_symbols, skips_single_symbol_and_returns_true_if_any_symbol_was_skipped)
{
	{
		std::string isString = "                       #";
		stringstream is(isString);
		Input input(is);
		EXPECT_TRUE(input.SkipCharacters({ SPACE_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), static_cast<long>(isString.length()));
	}
	{
		stringstream is("#");
		Input input(is);
		EXPECT_FALSE(input.SkipCharacters({ SPACE_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\n\n\n\n\n\n\n\n\n\n\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipCharacters({ ENDL_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 12);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("#");
		Input input(is);
		EXPECT_FALSE(input.SkipCharacters({ ENDL_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("#");
		Input input(is);
		EXPECT_TRUE(input.SkipCharacters({ '#' }));
		EXPECT_EQ(is.peek(), std::ifstream::traits_type::eof());
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 2);
	}
}

TEST(skip_symbols, skips_multiple_symbols)
{
	{
		stringstream is("  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		Input input(is);
		EXPECT_TRUE(input.SkipCharacters({ SPACE_SYMBOL, ENDL_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 12);
		EXPECT_EQ(input.GetPosition().GetColumn(), 4);
	}
	{
		stringstream is("#");
		Input input(is);
		EXPECT_FALSE(input.SkipCharacters({ SPACE_SYMBOL, ENDL_SYMBOL }));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 1);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
}
