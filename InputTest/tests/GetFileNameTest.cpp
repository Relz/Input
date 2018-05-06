#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(get_file_name, returns_empty_string_if_no_input_file_name_was_specified)
{
	wstringstream stringStream(L"not empty");
	EXPECT_EQ(Input(stringStream).GetFileName(), "");
}

TEST(get_file_name, returns_specified_input_file_name)
{
	string const fileName = "not_empty_file";
	ofstream wof(fileName, ofstream::trunc);
	wof.write("0", 1);
	wof.close();
	EXPECT_EQ(Input(fileName).GetFileName(), fileName);
}
