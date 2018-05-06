#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_line, returns_false_and_string_until_end_of_stream_if_end_of_line_was_not_reached)
{
	{
		wstringstream stringStream(L"0");
		Input input(stringStream);

		wstring line;
		EXPECT_FALSE(input.ReadLine(line));
		EXPECT_EQ(line, L"0");
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0\nstring");
		Input input(stringStream);

		wstring firstLine;
		EXPECT_TRUE(input.ReadLine(firstLine));
		EXPECT_EQ(firstLine, L"0");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L's', false, false));

		wstring secondLine;
		EXPECT_FALSE(input.ReadLine(secondLine));
		EXPECT_EQ(secondLine, L"string");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 7, false, 0, false, true));
	}
}

TEST(read_line, returns_true_and_string_until_end_of_line_if_end_of_line_was_reached)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument\n");
		Input input(stringStream);

		wstring line;
		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, L"0 0.1 stringArgument");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 0.1\nstringArgument");
		Input input(stringStream);

		wstring line;
		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, L"0 0.1");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L's', false, false));
	}
	{
		wstringstream stringStream(L"0\n0.1\nstringArgument");
		Input input(stringStream);
		wstring line;

		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, L"0");
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, false));

		EXPECT_TRUE(input.ReadLine(line));
		EXPECT_EQ(line, L"0.1");
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, false, 0, false, false));
	}
}

TEST(read_line, overwrites_passed_parameter_by_default)
{
	wstringstream stringStream(L"0 0.1 stringArgument\n");
	Input input(stringStream);

	wstring line = L"some string";
	EXPECT_TRUE(input.ReadLine(line));
	EXPECT_EQ(line, L"0 0.1 stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
}

TEST(read_line, can_append_line_to_passed_parameter)
{
	wstringstream stringStream(L"0 0.1 stringArgument\n");
	Input input(stringStream);

	wstring line = L"some string";
	EXPECT_TRUE(input.ReadLine(line, true));
	EXPECT_EQ(line, L"some string0 0.1 stringArgument");
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
}
