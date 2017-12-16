#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_vector_template_function, returns_false_if_can_not_read_any_element)
{
	wstringstream wis(L"0");
	CInput input(wis);
	int arg0;
	vector<int> numbers;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(numbers));
}

TEST(read_vector_template_function, returns_false_if_stream_can_not_convert_input_element)
{
	wstringstream wis(L"string");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_FALSE(input.ReadVector(numbers));
}

TEST(read_vector_template_function, reads_until_unexpected_type_element)
{
	vector<int> expectedVector = {0, 1, 2, 3};
	wstringstream wis(L"0 1 2 3 string");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, skip_whitespaces_if_non_character_type)
{
	vector<int> expectedVector = {0, 1, 2, 3};
	wstringstream wis(L"                                          0 1 2 3");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, dont_skip_empty_lines_by_default)
{
	vector<int> expectedVector = {};
	wstringstream wis(L"\n                             \n\n\n             0 1 2 3");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, push_back_by_default)
{
	vector<int> expectedVector = {0, 1, 2, 3};
	wstringstream wis(L"0 1 2 3");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_push_front)
{
	vector<int> expectedVector = {3, 2, 1, 0};
	wstringstream wis(L"0 1 2 3");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(
			numbers,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_FRONT,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, int>()
			}
	));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_skip_empty_lines)
{
	vector<int> expectedVector = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	wstringstream wis(L"0 1 2 3\n4 5 6 7\n\n8 9 10 11");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(
			numbers,
			{
					{'\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, int>()
			}
	));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, can_have_limit)
{
	vector<int> expectedVector = {0, 1, 2};
	wstringstream wis(L"0 1 2 3");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(
			numbers,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					3,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, int>()
			}
	));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, throws_exception_if_rules_and_true_char_are_not_specified)
{
	wstringstream wis(L"0 1 2 3");
	CInput input(wis);
	vector<bool> numbers;
	EXPECT_THROW(input.ReadVector(
			numbers,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, bool>()
			}
	), std::invalid_argument);
}

TEST(read_vector_template_function, reads_until_end_of_line_lf)
{
	vector<int> expectedVector = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	wstringstream wis(L"  1   2   3   4   5   6   7   8   9\n1 2");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, reads_until_end_of_line_cr)
{
	vector<int> expectedVector = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	wstringstream wis(L"  1   2   3   4   5   6   7   8   9\r1 2");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}

TEST(read_vector_template_function, reads_until_end_of_line_crlf)
{
	vector<int> expectedVector = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	wstringstream wis(L"  1   2   3   4   5   6   7   8   9\r\n1 2");
	CInput input(wis);
	vector<int> numbers;
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedVector);
}
