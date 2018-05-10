#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_line, returns_false_and_string_until_end_of_stream_if_end_of_line_was_not_reached)
{
	{
		stringstream stringStream("0");
		Input input(stringStream);

		string line;
		EXPECT_FALSE(input.ReadLine(line));
		EXPECT_EQ(line, "0");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("0\nstring");
		Input input(stringStream);

		string firstLine;
		EXPECT_TRUE(input.ReadLine(firstLine));
		EXPECT_EQ(firstLine, "0");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, 's', false, false));

		string secondLine;
		EXPECT_FALSE(input.ReadLine(secondLine));
		EXPECT_EQ(secondLine, "string");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 7, false, 0, false, true));
	}
}

TEST(read_line, returns_true_and_string_until_end_of_line_if_end_of_line_was_reached)
{
	{
		stringstream stringStream("0 0.1 stringArgument\n");
		Input input(stringStream);

		string line;
		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, "0 0.1 stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
	{
		stringstream stringStream("0 0.1\nstringArgument");
		Input input(stringStream);

		string line;
		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, "0 0.1");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, 's', false, false));
	}
	{
		stringstream stringStream("0\n0.1\nstringArgument");
		Input input(stringStream);
		string line;

		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, "0");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, false));

		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, "0.1");
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, false, 0, false, false));
	}
}

TEST(read_line, overwrites_passed_parameter_by_default)
{
	stringstream stringStream("0 0.1 stringArgument\n");
	Input input(stringStream);

	string line = "some string";
	EXPECT_TRUE(input.ReadLine(line));
	EXPECT_EQ(line, "0 0.1 stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
}

TEST(read_line, can_append_line_to_passed_parameter)
{
	stringstream stringStream("0 0.1 stringArgument\n");
	Input input(stringStream);

	string line = "some string";
	EXPECT_TRUE(input.ReadLine(line, true));
	EXPECT_EQ(line, "some string0 0.1 stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
}
