#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(constructor, throws_invalid_argument_if_file_does_not_exists)
{
	EXPECT_THROW(CInput("input/not_existing"), invalid_argument);
}

TEST(constructor, throws_invalid_argument_if_stream_is_empty)
{
	ofstream wof("empty", ofstream::trunc);
	wof.close();
	EXPECT_THROW(CInput("empty"), invalid_argument);
	stringstream is("");
	EXPECT_THROW(CInput input(is), invalid_argument);
}

TEST(constructor, initial_position_is_first_line_and_first_column)
{
	stringstream is("I am not empty");
	CInput input(is);
	EXPECT_EQ(input.GetPosition(), StreamPosition(1, 1));
}
