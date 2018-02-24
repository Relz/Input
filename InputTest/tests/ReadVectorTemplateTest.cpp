#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_vector_template, returns_false_and_does_not_modify_vector_if_can_not_read_any_element)
{
	stringstream is("0");
	Input input(is);
	int arg0;
	vector<int> numbers {1};
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, vector({1}));
}

TEST(read_vector_template, returns_false_and_does_not_modify_vector_if_stream_can_not_convert_input_element)
{
	stringstream is("string");
	Input input(is);
	vector<int> numbers {1};
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, vector({1}));
}

TEST(read_vector_template, reads_until_unexpected_type_element)
{
	vector<int> expectedVector {0, 1, 2, 3};
	stringstream is("0 1 2 3 string");
	Input input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, skip_whitespaces_if_non_character_type)
{
	vector<int> expectedVector {0, 1, 2, 3};
	stringstream is("                                          0 1 2 3");
	Input input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, dont_skip_empty_lines_by_default)
{
	vector<int> expectedVector {};
	stringstream is("\n                             \n\n\n             0 1 2 3");
	Input input(is);
	vector<int> numbers;
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, push_back_by_default)
{
	vector<int> expectedVector {4, 0, 1, 2, 3};
	stringstream is("0 1 2 3");
	Input input(is);
	vector<int> numbers {4};
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, can_push_front)
{
	vector<int> expectedVector {3, 2, 1, 0, 4};
	stringstream is("0 1 2 3");
	Input input(is);
	vector<int> numbers {4};
	EXPECT_TRUE(
		input.ReadVector(
			numbers,
			VectorSettingsBuilder<int>()
				.Build(
					BaseSettingsBuilder()
						.SetReadMethod(ReadVectorMethod::PUSH_FRONT)
						.Build()
				)
		)
	);
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, can_skip_empty_lines)
{
	vector<int> expectedVector {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	stringstream is("0 1 2 3\n4 5 6 7\n\n8 9 10 11");
	Input input(is);
	vector<int> numbers;
	EXPECT_TRUE(
		input.ReadVector(
			numbers,
			VectorSettingsBuilder<int>()
				.Build(
					BaseSettingsBuilder()
						.SetSkipCharacters({'\n'})
						.Build()
				)
		)
	);
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, can_have_limit)
{
	vector<int> expectedVector {0, 1, 2};
	stringstream is("0 1 2 3");
	Input input(is);
	vector<int> numbers;
	EXPECT_TRUE(
		input.ReadVector(
			numbers,
			VectorSettingsBuilder<int>()
					.Build(
							BaseSettingsBuilder()
									.SetReadLimit(3)
									.Build()
					)
		)
	);
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template, throws_exception_if_rules_and_true_char_are_not_specified)
{
	stringstream is("0 1 2 3");
	Input input(is);
	vector<bool> numbers;
	EXPECT_THROW(input.ReadVector(numbers), invalid_argument);
}

TEST(read_vector_template, reads_until_end_of_line_lf)
{
	{
		vector<int> expectedVector1 {1, 2, 3, 4, 5, 6, 7, 8, 9};
		vector<int> expectedVector2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2};
		stringstream is("  1   2   3   4   5   6   7   8   9\n1 2");
		Input input(is);
		vector<int> numbers;
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector2);
	}
	{
		vector<int> expectedVector1 {1, 2, 3, 4, 5, 6, 7, 8, 9};
		vector<int> expectedVector2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2};
		stringstream is("  1   2   3   4   5   6   7   8   9\n\r1 2");
		Input input(is);
		vector<int> numbers;
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector2);
	}
}

TEST(read_vector_template, reads_until_end_of_line_cr)
{
	{
		vector<int> expectedVector1 {1, 2, 3, 4, 5, 6, 7, 8, 9};
		vector<int> expectedVector2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2};
		stringstream is("  1   2   3   4   5   6   7   8   9\r1 2");
		Input input(is);
		vector<int> numbers;
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector2);
	}
	{
		vector<int> expectedVector1 {1, 2, 3, 4, 5, 6, 7, 8, 9};
		vector<int> expectedVector2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2};
		stringstream is("  1   2   3   4   5   6   7   8   9\r\n1 2");
		Input input(is);
		vector<int> numbers;
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector1);
		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedVector2);
	}
}
