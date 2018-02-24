#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_line, returns_false_if_stream_has_not_new_line)
{
	std::string isString = "0 0.1 stringArgument";
	stringstream is(isString);
	Input input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().GetLine(), 1);
	EXPECT_EQ(input.GetPosition().GetColumn(), isString.length() + 1);
}

TEST(skip_line, returns_false_if_new_line_in_end_of_stream_lf)
{
	stringstream is("...\n");
	Input input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().GetLine(), 2);
	EXPECT_EQ(input.GetPosition().GetColumn(), 1);
}

TEST(skip_line, returns_false_if_new_line_in_end_of_stream_cr)
{
	stringstream is("...\r");
	Input input(is);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(input.GetPosition().GetLine(), 2);
	EXPECT_EQ(input.GetPosition().GetColumn(), 1);
}

TEST(skip_line, returns_false_if_new_line_in_end_of_stream_crlf)
{
	stringstream is("...\r\n");
	Input input(is);
	EXPECT_TRUE(input.SkipLine());
	EXPECT_EQ(is.peek(), '\n');
	EXPECT_EQ(input.GetPosition().GetLine(), 2);
	EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	EXPECT_FALSE(input.SkipLine());
	EXPECT_EQ(is.peek(), std::ifstream::traits_type::eof());
	EXPECT_EQ(input.GetPosition().GetLine(), 3);
	EXPECT_EQ(input.GetPosition().GetColumn(), 1);
}

TEST(skip_line, skips_line_lf)
{
	{
		stringstream is("0 0.1 stringArgument \n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\n\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
}

TEST(skip_line, skips_line_cr)
{
	{
		stringstream is("0 0.1 stringArgument \r#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\r#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\r\r#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\r'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
}

TEST(skip_line, skips_line_crlf)
{
	{
		stringstream is("0 0.1 stringArgument \r\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 3);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\r\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 3);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
	{
		stringstream is("\r\n\r\n#");
		Input input(is);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 2);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\r'));
		EXPECT_EQ(input.GetPosition().GetLine(), 3);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
		EXPECT_EQ(input.GetPosition().GetLine(), 4);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('#'));
		EXPECT_EQ(input.GetPosition().GetLine(), 5);
		EXPECT_EQ(input.GetPosition().GetColumn(), 1);
	}
}
