#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const std::string SPACE_STRING = " ";
const std::string ENDL_STRING = "\n";

TEST(skip_until_strings_function, skips_until_strings_and_returns_true_if_any_symbol_was_reached)
{
	{
		stringstream is("                #       #$a");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({"#$"}, skippedString));
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 25);
		EXPECT_EQ(skippedString, "                #       ");
		char tmp;
		input.ReadArguments(tmp);
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('$'));
	}
	{
		stringstream is("#");
		CInput input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilStrings({SPACE_STRING}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 2);
		EXPECT_EQ(skippedString, "#");
	}
	{
		stringstream is("%\n\n\n\n\n\n\n\n\n\n\n#");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({"#"}, skippedString));
		EXPECT_EQ(input.GetPosition().line, 12);
		EXPECT_EQ(input.GetPosition().column, 1);
		char tmp;
		input.ReadArguments(tmp, tmp);
		EXPECT_TRUE(is.eof());
	}
}

TEST(skip_until_strings_function, skips_multiple_strings)
{
	{
		stringstream is("  abcaa\n      \n\n\n\n\n   \n\n\n\n   #");
		CInput input(is);
		std::string skippedString;
		EXPECT_TRUE(input.SkipUntilStrings({"aa", ENDL_STRING}, skippedString));
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 6);
		EXPECT_EQ(skippedString, "  abc");
		char tmp;
		input.ReadArguments(tmp, tmp);
		EXPECT_EQ(is.peek(), char_traits<char>::to_int_type('\n'));
	}
	{
		stringstream is("#");
		CInput input(is);
		std::string skippedString;
		EXPECT_FALSE(input.SkipUntilStrings({SPACE_STRING, ENDL_STRING}, skippedString));
		EXPECT_TRUE(is.eof());
		EXPECT_EQ(input.GetPosition().line, 1);
		EXPECT_EQ(input.GetPosition().column, 2);
		EXPECT_EQ(skippedString, "#");
	}
}
