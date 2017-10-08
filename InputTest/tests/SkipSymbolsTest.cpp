#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const wchar_t SPACE_SYMBOL = ' ';
const wchar_t ENDL_SYMBOL = '\n';

TEST(skip_symbols_function, skips_single_symbol)
{
	{
		wstringstream wis(L"                       #");
		CInput input(wis);
		input.SkipSymbols({SPACE_SYMBOL});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"#");
		CInput input(wis);
		input.SkipSymbols({SPACE_SYMBOL});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\n\n\n\n\n\n\n\n\n\n\n#");
		CInput input(wis);
		input.SkipSymbols({'\n'});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"#");
		CInput input(wis);
		input.SkipSymbols({'\n'});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
}

TEST(skip_symbols_function, skips_multiple_symbols)
{
	{
		wstringstream wis(L"  \n\n      \n\n\n\n\n   \n\n\n\n   #");
		CInput input(wis);
		input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"#");
		CInput input(wis);
		input.SkipSymbols({SPACE_SYMBOL, ENDL_SYMBOL});
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
}
