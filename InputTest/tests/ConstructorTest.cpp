#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(constructor, throws_invalid_argument_if_file_does_not_exists)
{
	EXPECT_THROW(Input("not_existing"), invalid_argument);
}

TEST(constructor, throws_invalid_argument_if_stream_is_empty)
{
	ofstream wof("empty", ofstream::trunc);
	wof.close();
	EXPECT_THROW(Input("empty"), invalid_argument);
	stringstream is("");
	EXPECT_THROW(Input input(is), invalid_argument);
}

TEST(constructor, initials_position_to_first_line_and_first_column)
{
	stringstream is("not empty");
	Input input(is);
	EXPECT_EQ(input.GetPosition().GetLine(), 1);
	EXPECT_EQ(input.GetPosition().GetColumn(), 1);
}
