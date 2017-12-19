#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const char SPACE_SYMBOL = ' ';
const char ENDL_SYMBOL = '\n';

TEST(skip_symbols_function, skips_single_symbol_and_returns_true_if_any_symbol_was_skipped)
{
	{
		stringstream is("                       #");
		CInput input(is);
		EXPECT_TRUE(input.SkipSymbols({SPACE_SYMBOL}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		EXPECT_FALSE(input.SkipSymbols({SPACE_SYMBOL}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("\n\n\n\n\n\n\n\n\n\n\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipSymbols({'\n'}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		EXPECT_FALSE(input.SkipSymbols({'\n'}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
}

TEST(skip_symbols_function, skips_multiple_symbols)
{
	{
		stringstream is("  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		CInput input(is);
		EXPECT_TRUE(input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		EXPECT_FALSE(input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL}));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
}
