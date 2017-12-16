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
}

TEST(read_arguments_function, returns_true_if_input_have_required_argument_count)
{
	stringstream is("0 0.1 stringArgument");
	CInput input(is);
	bool arg0;
	double arg1;
	string arg2;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1, arg2));
}

TEST(read_arguments_function, returns_true_if_input_have_enough_arguments)
{
	stringstream is("0 0.1 stringArgument");
	CInput input(is);
	bool arg0;
	double arg1;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1));
}

TEST(read_arguments_function, reads_required_arguments_from_stream)
{
	stringstream is("0 0.1 stringArgument");
	CInput input(is);
	bool arg0;
	double arg1;
	string arg2;
	input.ReadArguments(arg0, arg1, arg2);
	EXPECT_FALSE(arg0);
	EXPECT_DOUBLE_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, "stringArgument");
}

TEST(read_arguments_function, reads_cyrillic_symbols)
{
	stringstream is("привет, это строковыеАргументы");
	CInput input(is);
	string arg0;
	string arg1;
	string arg2;
	input.ReadArguments(arg0, arg1, arg2);
	EXPECT_EQ(arg0, "привет,");
	EXPECT_EQ(arg1, "это");
	EXPECT_EQ(arg2, "строковыеАргументы");
}
