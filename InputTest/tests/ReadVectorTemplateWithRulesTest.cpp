#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

const unordered_map<wchar_t, bool> RULES_BOOLS = {
		{'.', false},
		{'#', true}
};

const unordered_map<wchar_t, size_t> RULES_NUMBERS = {
		{'.', 0},
		{'#', 1},
		{'@', 2},
};

const unordered_map<wchar_t, wchar_t> RULES_SYMBOLS = {
		{'.', 'A'},
		{'#', 'B'},
		{'@', 'C'},
};

TEST(read_vector_template_with_rules_function, returns_false_if_can_not_read_any_element)
{
	wstringstream wis(L"0");
	CInput input(wis);
	int arg0;
	vector<bool> booleans;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
}

TEST(read_vector_template_with_rules_function, returns_false_if_can_not_convert_input_element)
{
	wstring wistreamStr = L"@.#.&#..#";
	{
		wstringstream wis(wistreamStr);
		CInput input(wis);
		vector<bool> booleans;
		EXPECT_FALSE(input.ReadVector(
				booleans,
				{
						std::vector<wchar_t>(),
						ReadVectorMethod::PUSH_BACK,
						ReadLimit::UNLIMITED,
						NOT_A_CHARACTER,
						RULES_BOOLS
				}
		));
	}
	{
		wstringstream wis(wistreamStr);
		CInput input(wis);
		vector<size_t> numbers;
		EXPECT_FALSE(input.ReadVector<wchar_t>(
				numbers,
				{
						std::vector<wchar_t>(),
						ReadVectorMethod::PUSH_BACK,
						ReadLimit::UNLIMITED,
						NOT_A_CHARACTER,
						RULES_NUMBERS
				}
		));
	}
	{
		wstringstream wis(wistreamStr);
		CInput input(wis);
		vector<wchar_t> symbols;
		EXPECT_FALSE(input.ReadVector(
				symbols,
				{
						std::vector<wchar_t>(),
						ReadVectorMethod::PUSH_BACK,
						ReadLimit::UNLIMITED,
						NOT_A_CHARACTER,
						RULES_SYMBOLS
				}
		));
	}
}

TEST(read_vector_template_with_rules_function, reads_until_unexpected_type_element)
{
	vector<bool> expectedVector = {false, true, false, true, false, false, true};
	wstringstream wis(L".#.#..#@");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_FALSE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules_function, push_back_by_default)
{
	vector<bool> expectedVector = {false, true, false, true, false, false, true};
	wstringstream wis(L".#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules_function, can_push_front)
{
	vector<bool> expectedVector = {true, false, false, true, false, true, false};
	wstringstream wis(L".#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_FRONT,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules_function, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	wstringstream wis(L"     \n\n\n             .#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_FALSE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
}

TEST(read_vector_template_with_rules_function, does_not_skip_before_first_allowed_value)
{
	vector<bool> expectedVector = {};
	wstringstream wis(L"     \n\n\n             .#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_FALSE(input.ReadVector(
			booleans,
			{
					{' ', '\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules_function, can_skip_whitespaces_and_empty_lines)
{
	vector<bool> expectedVector = {false, true, false, true, false, false, true};
	wstringstream wis(L". #     \n\n\n             .#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					{' ', '\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules_function, can_have_limit)
{
	vector<bool> expectedVector = {false, true, false, true};
	wstringstream wis(L".#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					4,
					NOT_A_CHARACTER,
					RULES_BOOLS
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}
