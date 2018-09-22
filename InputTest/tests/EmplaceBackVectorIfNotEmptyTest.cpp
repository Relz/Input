#include "Input.h"
#include "gtest/gtest.h"

using namespace std;

TEST(emplace_back_vector_if_not_empty, returns_true_if_vector_is_not_empty_and_emplace_back_vector_to_matrix)
{
	vector<vector<int>> matrix { };

	vector<int> vector1 { 1, 2, 3 };
	vector<int> vector2 { 4, 5, 6 };

	EXPECT_TRUE(Input::EmplaceBackVectorIfNotEmpty(matrix, vector1));
	EXPECT_EQ(matrix, vector<vector<int>>({ vector<int>({ 1, 2, 3 }) }));

	EXPECT_TRUE(Input::EmplaceBackVectorIfNotEmpty(matrix, vector2));
	EXPECT_EQ(matrix, vector<vector<int>>({ vector<int>({ 1, 2, 3 }), vector<int>({ 4, 5, 6 }) }));
}

TEST(emplace_back_vector_if_not_empty, returns_false_if_vector_is_empty_and_does_not_modify_matrix)
{
	vector<vector<int>> matrix { };

	vector<int> emptyVector { };

	EXPECT_FALSE(Input::EmplaceBackVectorIfNotEmpty(matrix, emptyVector));
	EXPECT_EQ(matrix, vector<vector<int>>({ }));
}
