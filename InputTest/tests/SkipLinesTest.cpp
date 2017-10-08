#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_lines_function, returns_false_if_stream_has_not_new_line_required_count_times)
{
	wstringstream wis(L"0 0.1 stringArgument \n#");
	CInput input(wis);
	EXPECT_FALSE(input.SkipLines(2));
}

TEST(skip_lines_function, skips_lines)
{
	{
		wstringstream wis(L"0 0.1 stringArgument \n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLines(1));
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
	{
		wstringstream wis(L"\n\n\n\n\n#");
		CInput input(wis);
		EXPECT_TRUE(input.SkipLines(5));
		EXPECT_EQ(wis.peek(), std::char_traits<wchar_t>::to_int_type('#'));
	}
}
