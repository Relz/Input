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

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_lf)
{
	wstringstream wis(L"...\n");
	CInput input(wis);
	EXPECT_FALSE(input.SkipLine());
}

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_cr)
{
	wstringstream wis(L"...\r");
	CInput input(wis);
	EXPECT_FALSE(input.SkipLine());
}

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_crlf)
{
	wstringstream wis(L"...\r\n");
	CInput input(wis);
	EXPECT_FALSE(input.SkipLine());
}

TEST(skip_line_function, skips_line_lf)
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

TEST(skip_line_function, skips_line_cr)
{
	{
		wstringstream wis(L"0 0.1 stringArgument \r#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\r#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\r\r#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('\r'));
	}
}

TEST(skip_line_function, skips_line_crlf)
{
	{
		wstringstream wis(L"0 0.1 stringArgument \r\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\r\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\r\n\r\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('\r'));
	}
}
