#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_vector_template_function, returns_false_if_can_not_read_any_element)
{
	stringstream is("0");
	CInput input(is);
	int arg0;
	vector<int> numbers;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(numbers));
}

TEST(read_vector_template_function, returns_false_if_stream_can_not_convert_input_element)
{
	stringstream is("string");
	CInput input(is);
	vector<int> numbers;
	EXPECT_FALSE(input.ReadVector(numbers));
}

TEST(read_vector_template_function, reads_until_unexpected_type_element)
{
	vector<int> expectedVector = { 0, 1, 2, 3 };
	stringstream is("0 1 2 3 string");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, skip_whitespaces_if_non_character_type)
{
	vector<int> expectedVector = { 0, 1, 2, 3 };
	stringstream is("                                          0 1 2 3");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, dont_skip_empty_lines_by_default)
{
	vector<int> expectedVector = {};
	stringstream is("\n                             \n\n\n             0 1 2 3");
	CInput input(is);
	vector<int> numbers;
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, push_back_by_default)
{
	vector<int> expectedVector = { 0, 1, 2, 3 };
	stringstream is("0 1 2 3");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_push_front)
{
	vector<int> expectedVector = { 3, 2, 1, 0 };
	stringstream is("0 1 2 3");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers,
		{ vector<char>(), ReadVectorMethod::PUSH_FRONT, ReadLimit::UNLIMITED, NOT_A_CHARACTER,
			unordered_map<char, int>() }));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_skip_empty_lines)
{
	vector<int> expectedVector = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	stringstream is("0 1 2 3\n4 5 6 7\n\n8 9 10 11");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers,
		{ { '\n' }, ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, NOT_A_CHARACTER, unordered_map<char, int>() }));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_have_limit)
{
	vector<int> expectedVector = { 0, 1, 2 };
	stringstream is("0 1 2 3");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(
		numbers, { vector<char>(), ReadVectorMethod::PUSH_BACK, 3, NOT_A_CHARACTER, unordered_map<char, int>() }));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, throws_exception_if_rules_and_true_char_are_not_specified)
{
	stringstream is("0 1 2 3");
	CInput input(is);
	vector<bool> numbers;
	EXPECT_THROW(input.ReadVector(numbers,
					 { vector<char>(), ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, NOT_A_CHARACTER,
						 unordered_map<char, bool>() }),
		invalid_argument);
}

TEST(read_vector_template_function, reads_until_end_of_line_lf)
{
	vector<int> expectedVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	stringstream is("  1   2   3   4   5   6   7   8   9\n1 2");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, reads_until_end_of_line_cr)
{
	vector<int> expectedVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	stringstream is("  1   2   3   4   5   6   7   8   9\r1 2");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, reads_until_end_of_line_crlf)
{
	vector<int> expectedVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	stringstream is("  1   2   3   4   5   6   7   8   9\r\n1 2");
	CInput input(is);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}
