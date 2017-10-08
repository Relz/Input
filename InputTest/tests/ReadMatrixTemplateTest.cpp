#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_matrix_template_function, returns_false_if_can_not_read_any_element)
{
	wstringstream wis(L"0");
	CInput input(wis);
	int arg0;
	vector<vector<int>> matrix;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadMatrix(matrix));
}

TEST(read_matrix_template_function, returns_false_if_stream_can_not_convert_input_element)
{
	wstringstream wis(L"string");
	CInput input(wis);
	vector<vector<int>> matrix;
	EXPECT_FALSE(input.ReadMatrix(matrix));
}

TEST(read_matrix_template_function, push_back_by_default)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
	CInput input("input/matrix");
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, can_push_front)
{
	vector<vector<int>> expectedMatrix = {{4, 5, 6, 7}, {0, 1, 2, 3}};
	CInput input("input/matrix");
	vector<vector<int>> numbers;
	EXPECT_TRUE(input.ReadMatrix(
			numbers,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_FRONT,
					ReadLimit::UNLIMITED
			}
	));
	EXPECT_EQ(numbers, expectedMatrix);
}

TEST(read_matrix_template_function, reads_until_unexpected_type_element_on_same_line)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}};
	CInput input("input/matrixWithUnexpectedElementOnSameLine");
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, reads_until_unexpected_type_element_on_new_line)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}};
	CInput input("input/matrixWithUnexpectedElementOnNewLine");
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, dont_skip_empty_lines_by_default)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}};
	wstringstream wis(L"0 1 2 3\n\n\n4 5 6 7");
	CInput input(wis);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, can_skip_empty_lines)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}};
	wstringstream wis(L"0 1 2 3\n\n4 5 6 7\n\n\n8 9 10 11");
	CInput input(wis);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(
			matrix,
			{
					{'\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED
			}
	));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, skip_whitespaces_if_non_character_elements)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
	wstringstream wis(L"  0 1 2 3\n   4 5 6 7");
	CInput input(wis);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template_function, can_have_limit)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
	wstringstream wis(L"0 1 2 3\n4 5 6 7\n8 9 10 11");
	CInput input(wis);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(
			matrix,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					2
			}
	));
	EXPECT_EQ(matrix, expectedMatrix);
}
