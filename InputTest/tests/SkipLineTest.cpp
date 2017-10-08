#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_line_function, returns_false_if_stream_has_not_new_line)
{
	wstringstream wis(L"0 0.1 stringArgument");
	CInput input(wis);
	EXPECT_FALSE(input.SkipLine());
}

TEST(skip_line_function, skips_line)
{
	{
		wstringstream wis(L"0 0.1 stringArgument \n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\n\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('\n'));
	}
}
