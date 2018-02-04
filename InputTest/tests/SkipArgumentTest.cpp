#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_argument_function, returns_false_if_stream_has_not_argument)
{
	stringstream is("0");
	CInput input(is);
	char firstArgument;
	input.ReadArguments(firstArgument);
	EXPECT_FALSE(input.SkipArgument<char>());
}

TEST(skip_argument_function, skips_argument)
{
	{
		stringstream is("0 0.1 stringArgument");
		CInput input(is);
		EXPECT_TRUE(input.SkipArgument<string>());

		double number;
		EXPECT_TRUE(input.ReadArguments(number));
		EXPECT_EQ(number, 0.1);
	}
	{
		stringstream is("0 0.1 stringArgument");
		CInput input(is);
		EXPECT_TRUE(input.SkipArgument<char>());

		char space;
		EXPECT_TRUE(input.ReadArguments(space));
		EXPECT_EQ(space, ' ');
	}
	{
		stringstream is("0 0.1 stringArgument");
		CInput input(is);
		EXPECT_TRUE(input.SkipArgument<string>());
		EXPECT_TRUE(input.SkipArgument<string>());

		string lastArgument;
		EXPECT_TRUE(input.ReadArguments(lastArgument));
		EXPECT_EQ(lastArgument, "stringArgument");
	}
}

TEST(skip_argument_function, does_not_skips_end_of_line)
{
	stringstream is("\n0 0.1 stringArgument");
	CInput input(is);
	EXPECT_FALSE(input.SkipArgument<char>());

	double number;
	EXPECT_FALSE(input.ReadArguments(number));
	EXPECT_NE(number, 0);
}
