#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_arguments_function, returns_false_if_input_does_not_have_enough_arguments)
{
	wstringstream wis(L"0");
	CInput input(wis);
	int arg0;
	int arg1;
	wstring str;
	EXPECT_FALSE(input.ReadArguments(arg0, arg1, str));
}

TEST(read_arguments_function, returns_true_if_input_have_required_argument_count)
{
	wstringstream wis(L"0 0.1 stringArgument");
	CInput input(wis);
	bool arg0;
	double arg1;
	wstring arg2;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1, arg2));
}

TEST(read_arguments_function, returns_true_if_input_have_enough_arguments)
{
	wstringstream wis(L"0 0.1 stringArgument");
	CInput input(wis);
	bool arg0;
	double arg1;
	EXPECT_TRUE(input.ReadArguments(arg0, arg1));
}

TEST(read_arguments_function, reads_required_arguments_from_stream)
{
	wstringstream wis(L"0 0.1 stringArgument");
	CInput input(wis);
	bool arg0;
	double arg1;
	wstring arg2;
	input.ReadArguments(arg0, arg1, arg2);
	EXPECT_FALSE(arg0);
	EXPECT_DOUBLE_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, L"stringArgument");
}
