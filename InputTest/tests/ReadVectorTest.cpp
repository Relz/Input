#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

unordered_map<wchar_t, bool> const RULES_BOOLEANS {
	{ L'.', false },
	{ L'#', true }
};

unordered_map<wchar_t, int> const RULES_NUMBERS {
	{ L'.', 0 },
	{ L'#', 1 },
	{ L'@', 2 },
};

unordered_map<wchar_t, wchar_t> const RULES_CHARACTERS {
	{ L'.', L'A' },
	{ L'#', L'B' },
	{ L'@', L'C' },
};

TEST(read_vector, returns_false_and_does_not_modify_passed_argument_if_can_not_read_any_element)
{
	{
		wstringstream stringStream(L"0");
		Input input(stringStream);

		vector<int> numbers { 1 };
		vector<int> expectedNumbers { 1 };
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0");
		Input input(stringStream);

		vector<wchar_t> characters { L'1' };
		vector<wchar_t> expectedCharacters { L'1' };
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_FALSE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_TRUE(input.SkipArgument<wchar_t>());
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"\n");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters;
		EXPECT_FALSE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, false, 0, true, false));
	}
}

TEST(read_vector, returns_false_and_does_not_modify_passed_argument_if_stream_can_not_convert_input_element)
{
	{
		wstringstream stringStream(L"string");
		Input input(stringStream);

		vector<int> numbers { 1 };
		vector<int> expectedNumbers { 1 };
		EXPECT_FALSE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L's', false, false));
	}
	{
		wstringstream stringStream(L"@.#.&#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '@', false, false));
	}
}

TEST(read_vector, reads_until_unexpected_type_element)
{
	{
		wstringstream stringStream(L"0 1 2 3 string");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 9, true, L's', false, false));
	}
}

TEST(read_vector, skip_whitespaces_if_non_character_type)
{
	wstringstream stringStream(L"     0 1 2 3");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers { 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 13, false, 0, false, true));
}

TEST(read_vector, does_not_skip_whitespaces_if_character_type)
{
	wstringstream stringStream(L"     0 1 23");
	Input input(stringStream);

	vector<wchar_t> characters;
	vector<wchar_t> expectedCharacters { L' ', L' ', L' ', L' ', L' ', L'0', L' ', L'1', L' ', L'2', L'3' };
	EXPECT_TRUE(input.ReadVector(characters));
	EXPECT_EQ(characters, expectedCharacters);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 12, false, 0, false, true));
}

TEST(read_vector, reads_end_of_lines_by_default)
{
	wstringstream stringStream(L"\n                             \n\n\n             0 1 2 3");
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
		wstringstream stringStream(L"0 1 2 3");
		Input input(stringStream);

		vector<int> numbers { 4 };
		vector<int> expectedNumbers { 4, 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"\n  \n\n  .#.#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters {
			L'\n',L' ', L' ', L'\n', L'\n', L' ', L' ', L'.', L'#', L'.', L'#', L'.', L'.', L'#' };
		EXPECT_TRUE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 10, false, 0, false, true));
	}
}

TEST(read_vector, can_push_front)
{
	{
		wstringstream stringStream(L"0 1 2 3");
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
		wstringstream stringStream(L".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { true, false, false, true, false, true, false };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0 1 23");
		Input input(stringStream);

		vector<wchar_t> characters { L'4' };
		vector<wchar_t> expectedCharacters { L'4', L'0', L' ', L'1', L' ', L'2', L'3' };
		EXPECT_TRUE(input.ReadVector(characters));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 7, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"0123");
		Input input(stringStream);

		vector<wchar_t> characters { L'4' };
		vector<wchar_t> expectedCharacters { L'3', L'2', L'1', L'0', L'4' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>()
				.Build(BaseSettingsBuilder().SetReadMethod(ReadVectorMethod::PUSH_FRONT).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, false, 0, false, true));
	}
}

TEST(read_vector, can_have_skip_characters)
{
	{
		wstringstream stringStream(L"     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetSkipCharacters({ L' ', L'\n' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 21, false, 0, false, true));
	}
	{
		wstringstream stringStream(L". #     \n\n\n             .#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetSkipCharacters({ L' ', L'\n' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 19, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters { L'.', L'#', L'.', L'#', L'.', L'.', L'#' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>()
				.Build(BaseSettingsBuilder().SetSkipCharacters({ L' ', L'\n' }).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 21, false, 0, false, true));
	}
	{
		wstringstream stringStream(L".#     \n\n\n             .#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacter { L'.', L'#', L'.', L'#', L'.', L'.', L'#' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>()
				.Build(BaseSettingsBuilder().SetSkipCharacters({ L' ', L'\n' }).Build())));
		EXPECT_EQ(characters, expectedCharacter);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 19, false, 0, false, true));
	}
}

TEST(read_vector, can_have_stop_characters)
{
	{
		wstringstream stringStream(L"#.#.$.#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { true, false, true, false };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetStopCharacters({ L'$' }).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, L'$', false, false));
	}
}

TEST(read_vector, can_have_rules)
{
	{
		wstringstream stringStream(L"     \n\n\n             .#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans;
		EXPECT_FALSE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L".#.#..#@");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, L'@', false, false));
	}
	{
		wstringstream stringStream(L"@.#.&#..#");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 2, 0, 1, 0 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, wchar_t>().SetRules(RULES_NUMBERS).Build()));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, L'&', false, false));
	}
	{
		wstringstream stringStream(L"@.#.&#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters { L'C', L'A', L'B', L'A' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>().SetRules(RULES_CHARACTERS).Build()));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, L'&', false, false));
	}
}

TEST(read_vector, can_do_not_stop_if_no_rule_and_have_default_value)
{
	{
		wstringstream stringStream(L".#%#X@#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true, false, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>().SetRules({{ L'#', true }}, false, false).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"    \n\n\n   .#.#..#");
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
			VectorSettingsBuilder<bool, wchar_t>().SetRules(RULES_BOOLEANS, false).Build()));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 4, 11, false, 0, false, true));
	}
}

TEST(read_vector, can_have_limit)
{
	{
		wstringstream stringStream(L"0 1 2 3");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadLimit(3).Build())));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 6, true, L' ', false, false));
	}
	{
		wstringstream stringStream(L".#.#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters { L'.', L'#', L'.' };
		EXPECT_TRUE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>().Build(BaseSettingsBuilder().SetReadLimit(3).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, L'#', false, false));
	}
	{
		wstringstream stringStream(L".#.#..#");
		Input input(stringStream);

		vector<bool> booleans;
		vector<bool> expectedBooleans { false, true, false, true };
		EXPECT_TRUE(input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool, wchar_t>()
				.SetRules(RULES_BOOLEANS)
				.Build(BaseSettingsBuilder().SetReadLimit(4).Build())));
		EXPECT_EQ(booleans, expectedBooleans);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 5, true, L'.', false, false));
	}
	{
		wstringstream stringStream(L".#.#..#");
		Input input(stringStream);

		vector<wchar_t> characters;
		vector<wchar_t> expectedCharacters;
		EXPECT_FALSE(input.ReadVector(
			characters,
			VectorSettingsBuilder<wchar_t, wchar_t>().Build(BaseSettingsBuilder().SetReadLimit(0).Build())));
		EXPECT_EQ(characters, expectedCharacters);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, L'.', false, false));
	}
}

TEST(read_vector, skips_end_of_line_by_default)
{
	{
		wstringstream stringStream(L"  1   2   3   4   5   6   7   8   9\n1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"  1   2   3   4   5   6   7   8   9\n\r1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };
		EXPECT_TRUE(input.ReadVector(numbers));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 3, 4, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"  1   2   3   4   5   6   7   8   9\r\n1 2");
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
		wstringstream stringStream(L"0 1 2 3\n4 5 6 7\n\n8 9 10 11");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, true, L'\n', true, false));
	}
	{
		wstringstream stringStream(L"  1   2   3   4   5   6   7   8   9\r1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers1 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		vector<int> expectedNumbers2 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, L'\r', true, false));

		EXPECT_FALSE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, L'\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'1', false, false));

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers2);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
	{
		wstringstream stringStream(L"  1   2   3   4   5   6   7   8   9\r\n1 2");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers1 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		vector<int> expectedNumbers2 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2 };

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, L'\r', true, false));

		EXPECT_FALSE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, true, L'\r', true, false));

		EXPECT_TRUE(input.SkipLine());
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 1, true, L'1', false, false));

		EXPECT_TRUE(input.ReadVector(
			numbers,
			VectorSettingsBuilder<int, int>().Build(BaseSettingsBuilder().SetReadEndOfLine(false).Build())));
		EXPECT_EQ(numbers, expectedNumbers2);
		EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
	}
}

TEST(read_vector, reads_string_vector_splitting_by_spaces)
{
	wstringstream stringStream(L" 1   2   3   4   5   6   7   8   9\r1 2");
	Input input(stringStream);

	vector<wstring> vect;
	vector<wstring> expectedVector { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"1", L"2" };
	EXPECT_TRUE(input.ReadVector(vect));
	EXPECT_EQ(vect, expectedVector);
	EXPECT_TRUE(TestHelper::CheckState(input, 2, 4, false, 0, false, true));
}
