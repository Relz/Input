#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_matrix_template, returns_false_and_does_not_modify_matrix_if_can_not_read_any_element)
{
	{
		stringstream stringStream("0");
		Input input(stringStream);

		vector<vector<int>> matrix {{ 1 }};
		vector<vector<int>> expectedMatrix {{ 1 }};
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_FALSE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("string");
		Input input(stringStream);

		vector<vector<int>> matrix {{ 1 }};
		vector<vector<int>> expectedMatrix {{ 1 }};
		EXPECT_FALSE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, 's', false, false));
	}
}

TEST(read_matrix_template, push_back_by_default)
{
	{
		stringstream stringStream("0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("0 1 2 3\n\n\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("0 1 2 3\n4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix { { 1 } };
		vector<vector<int>> expectedMatrix { { 1 }, { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3\n4 5 6 7");
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
		stringstream stringStream("0 1 2 3 String");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 9, true, 'S', false, false));
	}
	{
		stringstream stringStream("0 1 2 3\nString");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, 'S', false, false));
	}
	{
		stringstream stringStream("0 1 2 3\n\nString");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, 'S', false, false));
	}
}

TEST(read_matrix_template, skip_whitespaces_if_non_character_elements)
{
	{
		stringstream stringStream("  0 1 2 3\n   4 5 6 7");
		Input input(stringStream);

		vector<vector<int>> matrix;
		vector<vector<int>> expectedMatrix { { 0, 1, 2, 3, 4, 5, 6, 7 } };
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 11, false, 0, false, true));
	}
	{
		stringstream stringStream("  0 1 2 3\n   4 5 6 7");
		Input input(stringStream);

		vector<vector<char>> matrix;
		vector<vector<char>> expectedMatrix {
			{
				' ',
				' ',
				'0',
				' ',
				'1',
				' ',
				'2',
				' ',
				'3',
				'\n',
				' ',
				' ',
				' ',
				'4',
				' ',
				'5',
				' ',
				'6',
				' ',
				'7'
			}
		};
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 11, false, 0, false, true));
	}
}

TEST(read_matrix_template, can_have_limit)
{
	stringstream stringStream("0 1 2 3\n4 5 6 7\n8 9 10 11");
	Input input(stringStream);

	vector<vector<int>> matrix;
	vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 }, { 4, 5, 6, 7 } };
	EXPECT_TRUE(input.ReadMatrix(
		matrix,
		MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetReadLimit(2).Build()),
		VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, '8', false, false));
}

TEST(read_matrix_template, can_have_stop_characters)
{
	stringstream stringStream("0123\n4567\n#89");
	Input input(stringStream);

	vector<vector<char>> matrix;
	vector<vector<char>> expectedMatrix { { '0', '1', '2', '3' }, { '4', '5', '6', '7' } };
	EXPECT_TRUE(input.ReadMatrix(
		matrix,
		MatrixSettingsBuilder().Build(BaseSettingsBuilder().SetStopCharacters({ '#' }).Build()),
		VectorSettingsBuilder<char, char>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 3, 1, true, '#', false, false));
}
