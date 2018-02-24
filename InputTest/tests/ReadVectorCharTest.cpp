#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_vector_char, returns_false_and_does_not_modify_vector_if_can_not_read_any_element)
{
	stringstream is("0");
	Input input(is);
	int arg0;
	vector<char> chars {'1'};
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(chars));
	EXPECT_EQ(chars, vector({'1'}));
}

TEST(read_vector_char, reads_by_char_and_push_back_by_default)
{
	vector<char> expectedVector {'4', '0', ' ', '1', ' ', '2', '3'};
	stringstream is("0 1 23");
	Input input(is);
	vector<char> chars {'4'};
	EXPECT_TRUE(input.ReadVector(chars));
	EXPECT_EQ(chars, expectedVector);
}

TEST(read_vector_char, can_push_front)
{
	vector<char> expectedVector {'3', '2', '1', '0', '4'};
	stringstream is("0123");
	Input input(is);
	vector<char> chars {'4'};
	EXPECT_TRUE(
		input.ReadVector(
			chars,
			VectorSettingsBuilder<char>()
				.Build(
					BaseSettingsBuilder()
						.SetReadMethod(ReadVectorMethod::PUSH_FRONT)
						.Build()
				)
		)
	);
	EXPECT_EQ(chars, expectedVector);
}

TEST(read_vector_char, reads_whitespaces_and_empty_lines)
{
	vector<char> expectedVector {'\n', ' ', ' ', ' ', '\n', '\n', '\n', ' ', ' ', '.', '#', '.', '#', '.', '.', '#'};
	stringstream is("\n   \n\n\n  .#.#..#");
	Input input(is);
	vector<char> chars;
	EXPECT_TRUE(input.ReadVector(chars));
	EXPECT_EQ(chars, expectedVector);
}

TEST(read_vector_char, skips_characters_before_reading_first_element)
{
	vector<char> expectedVector {'.', '#', '.', '#', '.', '.', '#'};
	stringstream is("     \n\n\n             .#.#..#");
	Input input(is);
	vector<char> chars;
	EXPECT_TRUE(
		input.ReadVector(
			chars,
				VectorSettingsBuilder<char>()
					.Build(
						BaseSettingsBuilder()
							.SetSkipCharacters({' ', '\n'}).Build()
					)
		)
	);
	EXPECT_EQ(chars, expectedVector);
}

TEST(read_vector_char, can_skip_whitespaces_and_empty_lines)
{
	vector<char> expectedVector {'.', '#', '.', '#', '.', '.', '#'};
	stringstream is(".#     \n\n\n             .#..#");
	Input input(is);
	vector<char> chars;
	EXPECT_TRUE(
		input.ReadVector(
			chars,
			VectorSettingsBuilder<char>()
				.Build(
					BaseSettingsBuilder()
						.SetSkipCharacters({ ' ', '\n' })
						.Build()
				)
		)
	);
	EXPECT_EQ(chars, expectedVector);
}

TEST(read_vector_char, can_have_limit)
{
	vector<char> expectedVector {'.', '#', '.'};
	stringstream is(".#.#..#");
	Input input(is);
	vector<char> chars;
	EXPECT_TRUE(
		input.ReadVector(
			chars,
			VectorSettingsBuilder<char>()
				.Build(
					BaseSettingsBuilder()
						.SetReadLimit(3)
						.Build()
				)
		)
	);
	EXPECT_EQ(chars, expectedVector);
}
