#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

unordered_map<char, bool> const RULES_BOOLEANS {
	{ '.', false },
	{ '#', true }
};

unordered_map<char, int> const RULES_NUMBERS {
	{ '.', 0 },
	{ '#', 1 },
	{ '@', 2 },
};

unordered_map<char, char> const RULES_CHARACTERS {
	{ '.', 'A' },
	{ '#', 'B' },
	{ '@', 'C' },
};

TEST(read_vector, returns_false_and_does_not_modify_passed_argument_if_can_not_read_any_element)
{
	{
		stringstream stringStream("0");
		Input input(stringStream);

		vector<int> numbers { 1 };
		vector<int> expectedNumbers { 1 };
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("0");
		Input input(stringStream);

		vector<char> characters { '1' };
		vector<char> expectedCharacters { '1' };
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_FALSE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("0");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_TRUE(input.SkipArgument<char>());
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		stringstream stringStream("\n");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters;
		EXPECT_FALSE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, false, 0, true, false));
	}
}

TEST(read_vector, returns_false_and_does_not_modify_passed_argument_if_stream_can_not_convert_input_element)
{
	{
		stringstream stringStream("string");
		Input input(stringStream);

		vector<int> numbers { 1 };
		vector<int> expectedNumbers { 1 };
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, 's', false, false));
	}
	{
		stringstream stringStream("@.#.&#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '@', false, false));
	}
}

TEST(read_vector, reads_until_unexpected_type_element)
{
	{
		stringstream stringStream("0 1 2 3 string");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 9, true, 's', false, false));
	}
}

TEST(read_vector, skip_whitespaces_if_non_character_type)
{
	stringstream stringStream("     0 1 2 3");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers { 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 13, false, 0, false, true));
}

TEST(read_vector, does_not_skip_whitespaces_if_character_type)
{
	stringstream stringStream("     0 1 23");
	Input input(stringStream);

	vector<char> characters;
	vector<char> expectedCharacters { ' ', ' ', ' ', ' ', ' ', '0', ' ', '1', ' ', '2', '3' };
	EXPECT_TRUE(input.ReadVector(characters));
	EXPECT_EQ(characters, expectedCharacters);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 12, false, 0, false, true));
}

TEST(read_vector, reads_end_of_lines_by_default)
{
	stringstream stringStream("\n                             \n\n\n             0 1 2 3");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers { 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 5, 21, false, 0, false, true));
}

TEST(read_vector, push_back_by_default)
{
	{
		stringstream stringStream("0 1 2 3");
		Input input(stringStream);

		vector<int> numbers { 4 };
		vector<int> expectedNumbers { 4, 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream(".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("\n  \n\n  .#.#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters {
			'\n',' ', ' ', '\n', '\n', ' ', ' ', '.', '#', '.', '#', '.', '.', '#' };
		EXPECT_TRUE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 10, false, 0, false, true));
	}
}

TEST(read_vector, can_push_front)
{
	{
		stringstream stringStream("0 1 2 3");
		Input input(stringStream);

		vector<int> numbers { 4 };
		vector<int> expectedNumbers { 3, 2, 1, 0, 4 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int ,int>()
				.Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream(".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { true, false, false, true, false, true, false };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("0 1 23");
		Input input(stringStream);

		vector<char> characters { '4' };
		vector<char> expectedCharacters { '4', '0', ' ', '1', ' ', '2', '3' };
		EXPECT_TRUE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 7, false, 0, false, true));
	}
	{
		stringstream stringStream("0123");
		Input input(stringStream);

		vector<char> characters { '4' };
		vector<char> expectedCharacters { '3', '2', '1', '0', '4' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>()
				.Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, false, 0, false, true));
	}
}

TEST(read_vector, can_have_skip_characters)
{
	{
		stringstream stringStream("     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetSkipCharacters({ ' ', '\n' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 21, false, 0, false, true));
	}
	{
		stringstream stringStream(". #     \n\n\n             .#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetSkipCharacters({ ' ', '\n' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 19, false, 0, false, true));
	}
	{
		stringstream stringStream("     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters { '.', '#', '.', '#', '.', '.', '#' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>()
				.Build(BaseSettingsBuilder().SetSkipCharacters({ ' ', '\n' }).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 21, false, 0, false, true));
	}
	{
		stringstream stringStream(".#     \n\n\n             .#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacter { '.', '#', '.', '#', '.', '.', '#' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>()
				.Build(BaseSettingsBuilder().SetSkipCharacters({ ' ', '\n' }).Build())));
		EXPECT_EQ(characters, expectedCharacter);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 19, false, 0, false, true));
	}
}

TEST(read_vector, can_have_stop_characters)
{
	{
		stringstream stringStream("#.#.$.#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { true, false, true, false };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetStopCharacters({ '$' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, '$', false, false));
	}
}

TEST(read_vector, can_have_rules)
{
	{
		stringstream stringStream("     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, ' ', false, false));
	}
	{
		stringstream stringStream(".#.#..#@");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, '@', false, false));
	}
	{
		stringstream stringStream("@.#.&#..#");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 2, 0, 1, 0 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, char>().SetRules(RULES_NUMBERS).Build()));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, '&', false, false));
	}
	{
		stringstream stringStream("@.#.&#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters { 'C', 'A', 'B', 'A' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>().SetRules(RULES_CHARACTERS).Build()));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, '&', false, false));
	}
}

TEST(read_vector, can_do_not_stop_if_no_rule_and_have_default_value)
{
	{
		stringstream stringStream(".#%#X@#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules({{ '#', true }}, false, false).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("    \n\n\n   .#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans {
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			true,
			false,
			true,
			false,
			false,
			true
		};
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>().SetRules(RULES_BOOLEANS, false).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 11, false, 0, false, true));
	}
}

TEST(read_vector, can_have_limit)
{
	{
		stringstream stringStream("0 1 2 3");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadLimit(3).Build())));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, ' ', false, false));
	}
	{
		stringstream stringStream(".#.#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters { '.', '#', '.' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>().Build(BaseSettingsBuilder().SetReadLimit(3).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, '#', false, false));
	}
	{
		stringstream stringStream(".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, char>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetReadLimit(4).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, '.', false, false));
	}
	{
		stringstream stringStream(".#.#..#");
		Input input(stringStream);

		vector<char> characters;
		vector<char> expectedCharacters;
		EXPECT_FALSE(input.ReadVector(
			characters,
			VectorSettingsBuilder<char, char>().Build(BaseSettingsBuilder().SetReadLimit(0).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '.', false, false));
	}
}

TEST(read_vector, skips_end_of_line_by_default)
{
	{
		stringstream stringStream("  1   2   3   4   5   6   7   8   9\n1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
	{
		stringstream stringStream("  1   2   3   4   5   6   7   8   9\n\r1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 4, false, 0, false, true));
	}
	{
		stringstream stringStream("  1   2   3   4   5   6   7   8   9\r\n1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
}

TEST(read_vector, can_do_not_skip_end_of_lines)
{
	{
		stringstream stringStream("0 1 2 3\n4 5 6 7\n\n8 9 10 11");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, '\n', true, false));
	}
	{
		stringstream stringStream("  1   2   3   4   5   6   7   8   9\r1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers1 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		vector<int> expectedNumbers2 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, '\r', true, false));

		EXPECT_FALSE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, '\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '1', false, false));

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers2);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
	{
		stringstream stringStream("  1   2   3   4   5   6   7   8   9\r\n1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers1 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		vector<int> expectedNumbers2 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, '\r', true, false));

		EXPECT_FALSE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, '\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, '1', false, false));

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers2);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
}

TEST(read_vector, reads_string_vector_splitting_by_spaces)
{
	stringstream stringStream(" 1   2   3   4   5   6   7   8   9\r1 2");
	Input input(stringStream);

	vector<string> vect;
	vector<string> expectedVector { "1", "2", "3", "4", "5", "6", "7", "8", "9", "1", "2" };
	EXPECT_TRUE(input.ReadVector(vect));
	EXPECT_EQ(vect, expectedVector);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
}
