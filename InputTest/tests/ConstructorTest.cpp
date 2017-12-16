#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(constructor, throws_invalid_argument_if_file_does_not_exists)
{
	EXPECT_THROW(CInput("input/not_existing"), invalid_argument);
}

TEST(constructor, throws_invalid_argument_if_stream_is_empty)
{
	wofstream wof("empty", std::ofstream::trunc);
	wof.close();
	EXPECT_THROW(CInput("empty"), invalid_argument);
	wstringstream wis(L"");
	EXPECT_THROW(CInput input(wis), invalid_argument);
}
