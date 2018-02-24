#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_vector_with_true_char_function, returns_false_if_can_not_read_any_element)
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
				.SetTrueChar('#')
				.Build()
		)
	);
}

TEST(read_vector_with_true_char_function, push_back_by_default)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is(".#%#X@#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>().SetTrueChar('#').Build()
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_push_front)
{
	vector<bool> expectedVector {true, false, false, true, false, true, false};
	stringstream is(".#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetTrueChar('#')
				.Build(
					BaseSettingsBuilder()
						.SetReadMethod(ReadVectorMethod::PUSH_FRONT)
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, skip_whitespaces_and_empty_lines)
{
	vector<bool> expectedVector {false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, false, true};
	stringstream is("    \n\n\n   .#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>().SetTrueChar('#').Build()
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_have_skip_characters)
{
	vector<bool> expectedVector {false, true, false, true, false, false, true};
	stringstream is("    \n\n\n   .#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetTrueChar('#')
				.Build(
					BaseSettingsBuilder()
						.SetSkipCharacters({ ' ', '\n' })
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_have_stop_characters)
{
	vector<bool> expectedVector {false, false, false, false, false, false, false, false, false, false, true};
	stringstream is("    \n\n\n   #.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetTrueChar('#')
				.Build(
					BaseSettingsBuilder()
						.SetStopCharacters({'.'})
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_have_limit)
{
	vector<bool> expectedVector {false, true, false};
	stringstream is(".#.#..#");
	Input input(is);
	vector<bool> booleans;
	EXPECT_TRUE(
		input.ReadVector(
			booleans,
			VectorSettingsBuilder<bool>()
				.SetTrueChar('#')
				.Build(
					BaseSettingsBuilder()
						.SetReadLimit(3)
						.Build()
				)
		)
	);
	EXPECT_EQ(booleans, expectedVector);
}
