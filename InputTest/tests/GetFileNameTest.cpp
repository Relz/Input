#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(get_file_name_function, returns_empty_string_if_no_input_file_name_was_specified)
{
	stringstream is("not empty");
	CInput input(is);
	EXPECT_EQ(input.GetFileName(), "");
}

TEST(get_file_name_function, returns_specified_input_file_name)
{
	ofstream wof("empty", ofstream::trunc);
	wof.close();
	CInput input("empty");
	EXPECT_EQ(input.GetFileName(), "empty");
}
