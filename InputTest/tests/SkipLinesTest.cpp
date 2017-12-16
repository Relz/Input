#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_lines_function, returns_false_if_stream_has_not_new_line_required_count_times)
{
	stringstream is("0 0.1 stringArgument \n#");
	CInput input(is);
	EXPECT_FALSE(input.SkipLines(2));
}

TEST(skip_lines_function, skips_lines)
{
	{
		stringstream is("0 0.1 stringArgument \n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLines(1));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
	{
		stringstream is("\n\n\n\n\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLines(5));
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
	}
}
