#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const char SPACE_SYMBOL = ' ';
const char ENDL_SYMBOL = '\n';

TEST(skip_symbols_function, skips_single_symbol)
{
	{
		stringstream is("                       #");
		CInput input(is);
		input.SkipSymbols({SPACE_SYMBOL});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		input.SkipSymbols({SPACE_SYMBOL});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("\n\n\n\n\n\n\n\n\n\n\n#");
		CInput input(is);
		input.SkipSymbols({'\n'});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		input.SkipSymbols({'\n'});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
}

TEST(skip_symbols_function, skips_multiple_symbols)
{
	{
		stringstream is("  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		CInput input(is);
		input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("#");
		CInput input(is);
		input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL});
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
}
