#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_line_function, returns_false_if_stream_has_not_new_line)
{
	std::string isString = "0 0.1 stringArgument";
	stringstream is(isString);
	CInput input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().line, 1);
	EXPECT_EQ(input.GetPosition().column, static_cast<long>(isString.length()) + 1);
}

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_lf)
{
	stringstream is("...\n");
	CInput input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().line, 2);
	EXPECT_EQ(input.GetPosition().column, 1);
}

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_cr)
{
	stringstream is("...\r");
	CInput input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().line, 2);
	EXPECT_EQ(input.GetPosition().column, 1);
}

TEST(skip_line_function, returns_false_if_new_line_in_end_of_stream_crlf)
{
	stringstream is("...\r\n");
	CInput input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().line, 2);
	EXPECT_EQ(input.GetPosition().column, 1);
}

TEST(skip_line_function, skips_line_lf)
{
	{
		stringstream is("0 0.1 stringArgument \n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\n\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
}

TEST(skip_line_function, skips_line_cr)
{
	{
		stringstream is("0 0.1 stringArgument \r#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\r#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\r\r#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\r'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
}

TEST(skip_line_function, skips_line_crlf)
{
	{
		stringstream is("0 0.1 stringArgument \r\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\r\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
	{
		stringstream is("\r\n\r\n#");
		CInput input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\r'));
		EXPECT_EQ(input.GetPosition().line, 2);
		EXPECT_EQ(input.GetPosition().column, 1);
	}
}
