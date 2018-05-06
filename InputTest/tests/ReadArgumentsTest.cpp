#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_arguments, returns_false_if_input_does_not_have_enough_arguments)
{
	wstringstream stringStream(L"0");
	Input input(stringStream);

	int arg0 = 1;
	int arg1 = 2;
	EXPECT_FALSE(input.ReadArguments(true, arg0, arg1));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 2);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(read_arguments, returns_true_if_input_have_enough_arguments)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument");
		Input input(stringStream);

		bool arg0;
		double arg1;
		EXPECT_TRUE(input.ReadArguments(true, arg0, arg1));
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L"0 0.1 stringArgument");
		Input input(stringStream);

		bool arg0;
		double arg1;
		wstring arg2;
		EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
		EXPECT_EQ(arg0, 0);
		EXPECT_EQ(arg1, 0.1);
		EXPECT_EQ(arg2, L"stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
	}
}

TEST(read_arguments, reads_required_arguments_from_stream)
{
	wstringstream stringStream(L"0 0.1 stringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1;
	wstring arg2;
	EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
	EXPECT_FALSE(arg0);
	EXPECT_DOUBLE_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, L"stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 21, false, 0, false, true));
}

TEST(read_arguments, reads_cyrillic_symbols)
{
	{
		wstringstream stringStream(L"привет, это строковыеАргументы");
		Input input(stringStream);

		wstring arg0;
		wstring arg1;
		wstring arg2;
		EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
		EXPECT_EQ(arg0, L"привет,");
		EXPECT_EQ(arg1, L"это");
		EXPECT_EQ(arg2, L"строковыеАргументы");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 31, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"привет");
		Input input(stringStream);

		wchar_t firstChar;
		wchar_t secondChar;
		wchar_t thirdChar;
		EXPECT_TRUE(input.ReadArguments(true, firstChar, secondChar, thirdChar));
		EXPECT_EQ(firstChar, L'п');
		EXPECT_EQ(secondChar, L'р');
		EXPECT_EQ(thirdChar, L'и');
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, L'в', false, false));
	}
}

TEST(read_arguments, skip_end_of_line_by_default)
{
	wstringstream stringStream(L"0\n0.1\nstringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1;
	wstring arg2;
	EXPECT_TRUE(input.ReadArguments(true, arg0, arg1, arg2));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 0.1);
	EXPECT_EQ(arg2, L"stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 15, false, 0, false, true));
}

TEST(read_arguments, can_do_not_skip_end_of_line)
{
	wstringstream stringStream(L"0\n0.1\nstringArgument");
	Input input(stringStream);

	bool arg0;
	double arg1 = 0.5;
	wstring arg2 = L"default value";
	EXPECT_FALSE(input.ReadArguments(false, arg0, arg1, arg2));
	EXPECT_EQ(arg0, 0);
	EXPECT_EQ(arg1, 0.5);
	EXPECT_EQ(arg2, L"default value");
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, true, L'\n', true, false));
}
