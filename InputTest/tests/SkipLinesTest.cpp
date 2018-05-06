#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_lines, returns_false_if_stream_has_not_end_of_lines_required_count_times)
{
	wstringstream stringStream(L"0 0.1\nstringArgument\nanotherString\n");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipLines(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, L'a', false, false));
	EXPECT_FALSE(input.SkipLines(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 4, 1, false, 0, false, true));
}

TEST(skip_lines, skips_lines)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument \n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLines(1));
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\n\n\n\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLines(5));
		EXPECT_TRUE(TestHelper::CheckState(input, 6, 1, true, L'#', false, false));
	}
}
