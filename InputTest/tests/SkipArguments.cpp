#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_arguments_function, returns_true_if_stream_has_enough_argument)
{
	{
		wstringstream wis(L"0 1 2");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArguments<std::wstring>(1));
	}
	{
		wstringstream wis(L"0 1 2");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArguments<std::wstring>(2));
	}
	{
		wstringstream wis(L"0 1 2");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArguments<std::wstring>(3));
	}
}

TEST(skip_arguments_function, returns_false_if_stream_has_not_enough_argument)
{
	wstringstream wis(L"0 1 2");
	CInput input(wis);
	EXPECT_FALSE(input.SkipArguments<std::wstring>(4));
}

TEST(skip_arguments_function, skips_argument)
{
	{
		wstringstream wis(L"0 0.1 stringArgument");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArguments<std::wstring>(2));

		std::wstring lastArgument;
		EXPECT_TRUE(input.ReadArguments(lastArgument));
		EXPECT_EQ(lastArgument, L"stringArgument");
	}
}

TEST(skip_arguments_function, does_not_skips_end_of_line)
{
	wstringstream wis(L"\n0 0.1 stringArgument");
	CInput input(wis);
	EXPECT_FALSE(input.SkipArguments<std::wstring>(2));

	std::wstring lastArgument;
	EXPECT_FALSE(input.ReadArguments(lastArgument));
	EXPECT_NE(lastArgument, L"stringArgument");
}
