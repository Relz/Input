#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(skip_argument_function, returns_false_if_stream_has_not_argument)
{
	wstringstream wis(L"0");
	CInput input(wis);
	wchar_t firstArgument;
	input.ReadArguments(firstArgument);
	EXPECT_FALSE(input.SkipArgument<wchar_t>());
}

TEST(skip_argument_function, skips_argument)
{
	{
		wstringstream wis(L"0 0.1 stringArgument");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArgument<std::wstring>());

		double number;
		EXPECT_TRUE(input.ReadArguments(number));
		EXPECT_EQ(number, 0.1);
	}
	{
		wstringstream wis(L"0 0.1 stringArgument");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArgument<wchar_t>());

		wchar_t space;
		EXPECT_TRUE(input.ReadArguments(space));
		EXPECT_EQ(space, ' ');
	}
	{
		wstringstream wis(L"0 0.1 stringArgument");
		CInput input(wis);
		EXPECT_TRUE(input.SkipArgument<std::wstring>());
		EXPECT_TRUE(input.SkipArgument<std::wstring>());

		std::wstring lastArgument;
		EXPECT_TRUE(input.ReadArguments(lastArgument));
		EXPECT_EQ(lastArgument, L"stringArgument");
	}
}

TEST(skip_argument_function, does_not_skips_end_of_line)
{
	wstringstream wis(L"\n0 0.1 stringArgument");
	CInput input(wis);
	EXPECT_FALSE(input.SkipArgument<wchar_t>());

	double number = -1;
	EXPECT_FALSE(input.ReadArguments(number));
	EXPECT_NE(number, 0);
}
