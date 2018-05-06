#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_argument, returns_false_if_stream_has_not_argument)
{
	wstringstream stringStream(L"0");
	Input input(stringStream);

	EXPECT_TRUE(input.SkipArgument<wchar_t>());
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));

	EXPECT_FALSE(input.SkipArgument<wchar_t>());
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(skip_argument, skips_argument)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L' ', false, false));

		double number;
		EXPECT_TRUE(input.ReadArguments(true, number));
		EXPECT_EQ(number, 0.1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"0 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L' ', false, false));

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L' ', false, false));

		wstring lastArgument;
		EXPECT_TRUE(input.ReadArguments(true, lastArgument));
		EXPECT_EQ(lastArgument, L"stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
	}
}

TEST(skip_argument, skips_end_of_line_by_default)
{
	{
		wstringstream stringStream(L"\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'3', false, false));
	}
	{
		wstringstream stringStream(L"   \n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 2, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"\n3\n0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 2, true, L'\n', true, false));

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 4, true, L' ', false, false));
	}
}

TEST(skip_argument, can_do_not_skip_end_of_line)
{
	{
		wstringstream stringStream(L"\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_FALSE(input.SkipArgument<wchar_t>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"1\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wchar_t>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));

		EXPECT_FALSE(input.SkipArgument<wchar_t>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"1\n3 0.1 stringArgument");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));

		EXPECT_FALSE(input.SkipArgument<wstring>(false));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
	}
}
