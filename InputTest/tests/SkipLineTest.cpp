#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(skip_line, returns_false_if_end_of_line_was_not_reached)
{
	{
		wstringstream stringStream(L"string");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipArgument<wstring>());
		EXPECT_FALSE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 7, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"string\n");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_FALSE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, false, 0, false, true));
	}
}

TEST(skip_line, skips_line_lf)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument \n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\n\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'\n', true, false));
	}
}

TEST(skip_line, skips_line_cr)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument \r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\r\r#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'\r', true, false));
	}
}

TEST(skip_line, skips_line_crlf)
{
	{
		wstringstream stringStream(L"0 0.1 stringArgument \r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L"\r\n\r\n#");
		Input input(stringStream);

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, L'#', false, false));
	}
}
