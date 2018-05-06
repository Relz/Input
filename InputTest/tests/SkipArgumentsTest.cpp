#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_arguments, returns_true_if_stream_has_enough_argument)
{
	{
		wstringstream stringStream(L"0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(1));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(2));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"0 1 2");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<int>(3));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, false, 0, false, true));
	}
}

TEST(skip_arguments, returns_false_if_stream_has_not_enough_argument)
{
	wstringstream stringStream(L"0 1 2");
	Input input(stringStream);

	EXPECT_FALSE(input.SkipArguments<int>(4));
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, false, 0, false, true));
}

TEST(skip_arguments, skips_argument)
{
	wstringstream stringStream(L"0 0.1 stringArgument");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArguments<wstring>(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L' ', false, false));

	wstring lastArgument;
	EXPECT_TRUE(input.ReadArguments(true, lastArgument));
	EXPECT_EQ(lastArgument, L"stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
}

TEST(skip_arguments, skips_end_of_line_by_default)
{
	wstringstream stringStream(L"\n0 0.1 stringArgument");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArguments<wstring>(2));
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 6, true, L' ', false, false));

	wstring lastArgument;
	EXPECT_TRUE(input.ReadArguments(true, lastArgument));
	EXPECT_EQ(lastArgument, L"stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 21, false, 0, false, true));
}

TEST(skip_arguments, can_do_not_skip_end_of_line)
{
	{
		wstringstream stringStream(L"\n0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipArguments<wstring>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"1ab\n0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArguments<wchar_t>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 3, true, L'b', false, false));

		EXPECT_FALSE(input.SkipArguments<wchar_t>(2, false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, L'\n', true, false));
	}
}
