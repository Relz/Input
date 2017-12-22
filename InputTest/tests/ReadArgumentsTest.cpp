#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_arguments_function, returns_false_if_input_does_not_have_enough_arguments)
{
	stringstream is("0");
	CInput input(is);
	int arg0;
	int arg1;
	string str;
	EXPECT_FALSE(input.ReadArguments(arg0, arg1, str));
	EXPECT_EQ(input.GetLine(), 1);
	EXPECT_EQ(input.GetColumn(), 2);
}

TEST(read_arguments_function, returns_true_if_input_have_required_argument_count)
{
	std::string isStr = "0 0.1 stringArgument";
	stringstream is(isStr);
	CInput input(is);
	bool arg0;
	double arg1;
	string arg2;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1, arg2));
	EXPECT_EQ(input.GetLine(), 1);
	EXPECT_EQ(input.GetColumn(), static_cast<long>(isStr.length()) + 1);
}

TEST(read_arguments_function, returns_true_if_input_have_enough_arguments)
{
	std::string isStr = "0 0.1 stringArgument";
	stringstream is(isStr);
	CInput input(is);
	bool arg0;
	double arg1;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1));
	EXPECT_EQ(input.GetLine(), 1);
	EXPECT_EQ(input.GetColumn(), 6);
}

TEST(read_arguments_function, reads_required_arguments_from_stream)
{
	std::string isStr = "0 0.1 stringArgument";
	stringstream is(isStr);
	CInput input(is);
	bool arg0;
	double arg1;
	string arg2;
	input.ReadArguments(arg0, arg1, arg2);
	EXPECT_FALSE(arg0);
	EXPECT_DOUBLE_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, "stringArgument");
	EXPECT_EQ(input.GetLine(), 1);
	EXPECT_EQ(input.GetColumn(), static_cast<long>(isStr.length()) + 1);
}

TEST(read_arguments_function, reads_cyrillic_symbols)
{
	std::string isStr = "привет, это строковыеАргументы";
	stringstream is(isStr);
	CInput input(is);
	string arg0;
	string arg1;
	string arg2;
	input.ReadArguments(arg0, arg1, arg2);
	EXPECT_EQ(arg0, "привет,");
	EXPECT_EQ(arg1, "это");
	EXPECT_EQ(arg2, "строковыеАргументы");
	EXPECT_EQ(input.GetLine(), 1);
	EXPECT_EQ(input.GetColumn(), static_cast<long>(isStr.length()) + 1);
}
