#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_matrix, returns_false_and_does_not_modify_matrix_if_can_not_read_any_element)
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

TEST(read_matrix, returns_false_and_does_not_modify_matrix_if_stream_can_not_convert_input_element)
{
	stringstream stringStream("string");
	Input input(stringStream);

	vector<vector<int>> matrix {{ 1 }};
	vector<vector<int>> expectedMatrix {{ 1 }};
	EXPECT_FALSE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, 's', false, false));
}

TEST(read_matrix, emplaces_back_read_vector_by_default)
{
	stringstream stringStream("0 1 2 3\n4 5 6 7");
	Input input(stringStream);

	vector<vector<int>> matrix;
	vector<vector<int>> expectedMatrix { { 0, 1, 2, 3 }, { 4, 5, 6, 7 } };
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
}

TEST(read_matrix, appends_vectors_to_matrix)
{
	stringstream stringStream("0 1 2 3\n4 5 6 7");
	Input input(stringStream);

	vector<vector<int>> matrix {{ 1 }};
	vector<vector<int>> expectedMatrix {{ 1 }, { 0, 1, 2, 3 }, { 4, 5, 6, 7 }};
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
}

TEST(read_matrix, stop_if_vector_is_empty)
{
	stringstream stringStream("0 1 2 3\n\n4 5 6 7");
	Input input(stringStream);

	vector<vector<int>> matrix;
	vector<vector<int>> expectedMatrix {{ 0, 1, 2, 3 }};
	EXPECT_TRUE(input.ReadMatrix(matrix));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '\n', true, false));
}

TEST(read_matrix, can_specify_custom_for_each_read_vector_behavior)
{
	stringstream stringStream("0 1 2 3\n4 5 6 7");
	Input input(stringStream);

	vector<vector<int>> matrix { };
	vector<vector<int>> expectedMatrix { { 4, 5, 6, 7 }, { 0, 1, 2, 3 } };

	function<bool(vector<vector<int>> &, vector<int> &)> emplaceFrontReadVector = [](vector<vector<int>> & matrix, vector<int> & vect)
	{
		if (vect.empty())
		{
			return false;
		}
		matrix.insert(matrix.begin(), vect);
		return true;
	};

	EXPECT_TRUE(input.ReadMatrix(matrix, false, &Input::EmplaceBackElement<int>, emplaceFrontReadVector));
	EXPECT_EQ(matrix, expectedMatrix);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 8, false, 0, false, true));
}
