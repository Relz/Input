#include "Input.h"
#include "gtest/gtest.h"

using namespace std;

TEST(get_char_position, returns_char_position)
{
	string const fileName = "not_empty_file";
	ofstream of(fileName, ofstream::trunc);
	of.write("0", 1);
	of.close();
	Input input(fileName);
	EXPECT_EQ(input.GetCharPosition(), 0);
	input.SkipArgument<char>();
	EXPECT_EQ(input.GetCharPosition(), 1);
	input.SkipArgument<char>();
	EXPECT_EQ(input.GetCharPosition(), 1);
}
