#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

unordered_map<char, bool> const RULES_BOOLEANS = {{'.', false}, {'#', true}};

unordered_map<char, size_t> const RULES_NUMBERS {
	{'.', 0},
	{'#', 1},
	{'@', 2},
};

unordered_map<char, char> const RULES_CHARACTERS {
	{'.', 'A'},
	{'#', 'B'},
	{'@', 'C'},
};

TEST(read_vector_template_with_rules, returns_false_if_can_not_read_any_element)
{
	stringstream is("0");
	Input input(is);
	int arg0;
	vector<bool> booleans;
	input.ReadArguments(arg0);
	EXPECT_FALSE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetRules(RULES_BOOLEANS)
				.Build()
		)
	);
}

TEST(read_vector_template_with_rules, returns_false_if_can_not_convert_any_element)
{
	string istreamStr = "@.#.&#..#";
	{
		stringstream is(istreamStr);
		Input input(is);
		vector<bool> booleans;
		EXPECT_FALSE(
			input.ReadVector(
				booleans,
				VectorSettingsBuilder<bool>().SetRules(RULES_BOOLEANS).Build()
			)
		);
		EXPECT_TRUE(booleans.empty());
	}
	{
		vector<size_t> expectedVector {2, 0, 1, 0};
		stringstream is(istreamStr);
		Input input(is);
		vector<size_t> numbers;
		EXPECT_FALSE(
			input.ReadVector<char>(
				numbers,
				VectorSettingsBuilder<size_t>().SetRules(RULES_NUMBERS).Build()
			)
		);
		EXPECT_EQ(numbers, expectedVector);
	}
	{
		vector<char> expectedVector { 'C', 'A', 'B', 'A' };
		stringstream is(istreamStr);
		Input input(is);
		vector<char> characters;
		EXPECT_FALSE(
			input.ReadVector(
				characters,
				VectorSettingsBuilder<char>().SetRules(RULES_CHARACTERS).Build()
			)
		);
		EXPECT_EQ(characters, expectedVector);
	}
}

TEST(read_vector_template_with_rules, reads_until_unexpected_type_element)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is(".#.#..#@");
	Input input(is);
	vector<bool> booleans;
	EXPECT_FALSE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>().SetRules(RULES_BOOLEANS).Build()
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules, push_back_by_default)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is(".#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>().SetRules(RULES_BOOLEANS).Build()
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules, can_push_front)
{
	vector<bool> expectedVector {true, false, false, true, false, true, false};
	stringstream is(".#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetRules(RULES_BOOLEANS)
				.Build(
					BaseSettingsBuilder()
						.SetReadMethod(ReadVectorMethod::PUSH_FRONT)
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	stringstream is("     \n\n\n             .#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_FALSE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>().SetRules(RULES_BOOLEANS).Build()
		)
	);
}

TEST(read_vector_template_with_rules, skips_before_first_reading)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is("     \n\n\n             .#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
				VectorSettingsBuilder<bool>()
					.SetRules(RULES_BOOLEANS)
					.Build(
						BaseSettingsBuilder()
							.SetSkipCharacters({' ', '\n'})
							.Build()
					)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules, can_skip_whitespaces_and_empty_lines)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is(". #     \n\n\n             .#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetRules(RULES_BOOLEANS)
				.Build(
					BaseSettingsBuilder()
						.SetSkipCharacters({' ', '\n'})
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_template_with_rules, can_have_limit)
{
	vector<bool> expectedVector {false, true, false, true};
	stringstream is(".#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetRules(RULES_BOOLEANS)
				.Build(
					BaseSettingsBuilder()
						.SetReadLimit(4)
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}
