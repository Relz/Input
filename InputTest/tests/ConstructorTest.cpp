#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(constructor, throws_invalid_argument_if_file_does_not_exists)
{
	EXPECT_THROW(Input("not_existing"), invalid_argument);
}

TEST(constructor, throws_invalid_argument_if_stream_is_empty)
{
	ofstream output("empty", ofstream::trunc);
	output.close();
	EXPECT_THROW(Input("empty"), invalid_argument);
	wstringstream stringStream(L"");
	EXPECT_THROW(Input input(stringStream), invalid_argument);
}

TEST(constructor, initials_position_to_first_line_and_first_column)
{
	wstringstream stringStream(L"not empty");
	Input input(stringStream);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'n', false, false));
}
