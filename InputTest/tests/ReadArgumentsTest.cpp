#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_arguments, returns_false_if_input_does_not_have_enough_arguments)
{
	stringstream stringStream("0");
	Input input(stringStream);

	int arg0 = 1;
	int arg1 = 2;
	EXPECT_FALSE(input.ReadArguments(true, arg0, arg1));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 2);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(read_arguments, returns_true_if_input_have_enough_arguments)
{
	{
		stringstream stringStream("0 0.1 stringArgument");
		Input input(stringStream);

		bool arg0;
		double arg1;
		EXPECT_TRUE(input.ReadArguments(true, arg0, arg1));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, ' ', false, false));
	}
	{
		stringstream stringStream("0 0.1 stringArgument");
		Input input(stringStream);

		bool arg0;
		double arg1;
		string arg2;
		EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
		EXPECT_EQ(arg0, 0);
		EXPECT_EQ(arg1, 0.1);
		EXPECT_EQ(arg2, "stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
	}
}

TEST(read_arguments, reads_required_arguments_from_stream)
{
	stringstream stringStream("0 0.1 stringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1;
	string arg2;
	EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
	EXPECT_FALSE(arg0);
	EXPECT_DOUBLE_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, "stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
}

TEST(read_arguments, skips_end_of_line_by_default)
{
	stringstream stringStream("0\n0.1\nstringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1;
	string arg2;
	EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, "stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 15, false, 0, false, true));
}

TEST(read_arguments, can_do_not_skip_end_of_line)
{
	stringstream stringStream("0\n0.1\nstringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1 = 0.5;
	string arg2 = "default value";
	EXPECT_FALSE(input.ReadArguments(false, arg0, arg1, arg2));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 0.5);
	EXPECT_EQ(arg2, "default value");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, '\n', true, false));
}
