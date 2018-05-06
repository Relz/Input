#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(is_end_of_line, returns_false_if_end_of_stream)
{
	wstringstream stringStream(L"#");
	Input input(stringStream);

	EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'#', false, false));
	EXPECT_TRUE(input.SkipArgument<wchar_t>());
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(is_end_of_line, returns_true_if_end_of_line)
{
	{
		wstringstream stringStream(L"\n");
		Input input(stringStream);

		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'\n', true, false));
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"\n\n");
		Input input(stringStream);

		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'\n', true, false));

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'\n', true, false));

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"#\n");
		Input input(stringStream);

		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'#', false, false));

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
	}
}
