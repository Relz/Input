#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const char SPACE_SYMBOL = ' ';
const char ENDL_SYMBOL = '\n';

TEST(skip_until_symbols_function, skips_until_symbols_and_returns_true_if_any_symbol_was_reached)
{
	{
		stringstream is("                       #");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilSymbols({'#'}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 24);
		EXPECT_EQ(skippedString, "                       ");
	}
	{
		stringstream is("#");
		CInput input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilSymbols({SPACE_SYMBOL}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 2);
		EXPECT_EQ(skippedString, "#");
	}
	{
		stringstream is("%\n\n\n\n\n\n\n\n\n\n\n#");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilSymbols({'\n'}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 2);
		EXPECT_EQ(skippedString, "%");
	}
}

TEST(skip_until_symbols_function, skips_multiple_symbols)
{
	{
		stringstream is("  \na\n      \n\n\n\n\n   \n\n\n\n   #");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilSymbols({'a', ENDL_SYMBOL}, skippedString));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 3);
		EXPECT_EQ(skippedString, "  ");
	}
	{
		stringstream is("#");
		CInput input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilSymbols({SPACE_SYMBOL, ENDL_SYMBOL}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 2);
		EXPECT_EQ(skippedString, "#");
	}
}
