#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_matrix_template, returns_false_and_does_not_modify_matrix_if_can_not_read_any_element)
{
	{
		wstringstream stringStream(L"0");
		Input input(stringStream);

		vector<vector<int>> matrix {{ 1 }};
		vector<vector<int>> expectedMatrix {{ 1 }};
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_FALSE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"string");
		Input input(stringStream);

		vector<vector<int>> matrix {{ 1 }};
		vector<vector<int>> expectedMatrix {{ 1 }};
		EXPECT_FALSE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L's', false, false));
	}
}

TEST(read_matrix_template, push_back_by_default)
{
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n\n\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix { { 1 } };
		vector<vector<int>> expectedMatrix { { 1 }, { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 }, { 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(
			matrix,
			MatrixSettings(),
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix { { 1 } };
		vector<vector<int>> expectedMatrix { { 1 }, { 0, 1, 2, 3 }, { 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(
			matrix,
			MatrixSettings(),
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
}

TEST(read_matrix_template, can_push_front)
{
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(
			matrix,
			MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> numbers { { 1 } };
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 }, { 1 } };
		EXPECT_TRUE(input.ReadMatrix(
			numbers,
			MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(numbers, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 4, 5, 6, 7 }, { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(
			matrix,
			MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build()),
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> numbers { { 1 } };
		vector<vector<int>> expectedMatrix { { 4, 5, 6, 7 }, { 0, 1, 2, 3 }, { 1 } };
		EXPECT_TRUE(input.ReadMatrix(
			numbers,
			MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build()),
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
}

TEST(read_matrix_template, reads_until_unexpected_type_element)
{
	{
		wstringstream stringStream(L"0 1 2 3 String");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 9, true, L'S', false, false));
	}
	{
		wstringstream stringStream(L"0 1 2 3\nString");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'S', false, false));
	}
	{
		wstringstream stringStream(L"0 1 2 3\n\nString");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, L'S', false, false));
	}
}

TEST(read_matrix_template, skip_whitespaces_if_non_character_elements)
{
	{
		wstringstream stringStream(L"  0 1 2 3\n   4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 11, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"  0 1 2 3\n   4 5 6 7");
		Input input(stringStream);

		vector<vector<wchar_t>> matrix;
		vector<vector<wchar_t>> expectedMatrix {
			{
				L' ',
				L' ',
				L'0',
				L' ',
				L'1',
				L' ',
				L'2',
				L' ',
				L'3',
				L'\n',
				L' ',
				L' ',
				L' ',
				L'4',
				L' ',
				L'5',
				L' ',
				L'6',
				L' ',
				L'7'
			}
		};
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 11, false, 0, false, true));
	}
}

TEST(read_matrix_template, can_have_limit)
{
	wstringstream stringStream(L"0 1 2 3\n4 5 6 7\n8 9 10 11");
	Input input(stringStream);

	vector<vector<int>> matrix;
	vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 }, { 4, 5, 6, 7 } };
	EXPECT_TRUE(input.ReadMatrix(
		matrix,
		MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadLimit(2).Build()),
		VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, L'8', false, false));
}

TEST(read_matrix_template, can_have_stop_characters)
{
	wstringstream stringStream(L"0123\n4567\n#89");
	Input input(stringStream);

	vector<vector<wchar_t>> matrix;
	vector<vector<wchar_t>> expectedMatrix { { L'0', L'1', L'2', L'3' }, { L'4', L'5', L'6', L'7' } };
	EXPECT_TRUE(input.ReadMatrix(
		matrix,
		MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetStopCharacters({ L'#' }).Build()),
		VectorSettingsBuilder<wchar_t, wchar_t>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, L'#', false, false));
}
