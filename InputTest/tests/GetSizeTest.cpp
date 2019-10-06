#include "Input.h"
#include "gtest/gtest.h"

using namespace std;

TEST(get_size, returns_input_file_size)
{
	string const fileName = "not_empty_file";
	ofstream of(fileName, ofstream::trunc);
	of.write("0", 1);
	of.close();
	EXPECT_EQ(Input(fileName).GetSize(), 1);
}
