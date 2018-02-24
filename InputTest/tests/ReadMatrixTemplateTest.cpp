#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_matrix_template, returns_false_and_does_not_modify_matrix_if_can_not_read_any_element)
{
	stringstream is("0");
	Input input(is);
	int arg0;
	vector<vector<int>> matrix = {{1}};
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, vector<vector<int>>(1, {1}));
}

TEST(read_matrix_template, returns_false_and_does_not_modify_matrix_if_stream_can_not_convert_input_element)
{
	stringstream is("string");
	Input input(is);
	vector<vector<int>> matrix = {{1}};
	EXPECT_FALSE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, vector<vector<int>>(1, {1}));
}

TEST(read_matrix_template, push_back_by_default_lf)
{
	{
		vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\n4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix;
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
	{
		vector<vector<int>> expectedMatrix = {{1}, {0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\n4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix = {{1}};
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
}

TEST(read_matrix_template, push_back_by_default_cr)
{
	{
		vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\r4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix;
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
	{
		vector<vector<int>> expectedMatrix = {{1}, {0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\r4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix = {{1}};
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
}

TEST(read_matrix_template, push_back_by_default_crlf)
{
	{
		vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\r\n4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix;
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
	{
		vector<vector<int>> expectedMatrix = {{1}, {0, 1, 2, 3}, {4, 5, 6, 7}};
		stringstream is("0 1 2 3\r\n4 5 6 7");
		Input input(is);
		vector<vector<int>> matrix = {{1}};
		EXPECT_TRUE(input.ReadMatrix(matrix));
		EXPECT_EQ(matrix, expectedMatrix);
	}
}

TEST(read_matrix_template, can_push_front)
{
	{
		vector<vector<int>> expectedMatrix = {{4, 5, 6, 7}, {0, 1, 2, 3}};
		stringstream is("0 1 2 3\n4 5 6 7");
		Input input(is);
		vector<vector<int>> numbers;
		EXPECT_TRUE(input.ReadMatrix(numbers, {{}, {}, ReadVectorMethod::PUSH_FRONT, ReadLimit::UNLIMITED}));
		EXPECT_EQ(numbers, expectedMatrix);
	}
	{
		vector<vector<int>> expectedMatrix = {{4, 5, 6, 7}, {0, 1, 2, 3}, {1}};
		stringstream is("0 1 2 3\n4 5 6 7");
		Input input(is);
		vector<vector<int>> numbers = {{1}};
		EXPECT_TRUE(input.ReadMatrix(numbers, {{}, {}, ReadVectorMethod::PUSH_FRONT, ReadLimit::UNLIMITED}));
		EXPECT_EQ(numbers, expectedMatrix);
	}
}

TEST(read_matrix_template, reads_until_unexpected_type_element_on_same_line)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}};
	stringstream is("0 1 2 3 String");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template, reads_until_unexpected_type_element_on_new_line)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}};
	stringstream is("0 1 2 3\nString");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template, skip_empty_lines_by_default)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
	stringstream is("0 1 2 3\n\n\n4 5 6 7");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template, skip_whitespaces_if_non_character_elements)
{
	vector<vector<int>> expectedMatrix = {{ 0, 1, 2, 3 }, { 4, 5, 6, 7 }};
	stringstream is("  0 1 2 3\n   4 5 6 7");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template, can_have_limit)
{
	vector<vector<int>> expectedMatrix = {{ 0, 1, 2, 3 }, { 4, 5, 6, 7 }};
	stringstream is("0 1 2 3\n4 5 6 7\n8 9 10 11");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix, {{}, {}, ReadVectorMethod::PUSH_BACK, 2}));
	EXPECT_EQ(matrix, expectedMatrix);
}

TEST(read_matrix_template, can_have_stop_characters)
{
	vector<vector<int>> expectedMatrix = {{0, 1, 2, 3}, {4, 5, 6, 7}};
	stringstream is("0 1 2 3\n\n4 5 6 7\n\n\n # 8 9 10 11");
	Input input(is);
	vector<vector<int>> matrix;
	EXPECT_TRUE(input.ReadMatrix(matrix, {{}, {'#'}, ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED}));
	EXPECT_EQ(matrix, expectedMatrix);
}
