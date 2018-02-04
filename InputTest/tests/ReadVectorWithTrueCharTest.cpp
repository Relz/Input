#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_vector_with_true_char_function, returns_false_if_can_not_read_any_element)
{
	stringstream is("0");
	CInput input(is);
	int arg0;
	vector<bool> booleans;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(booleans,
		{ vector<char>(), ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, '#', unordered_map<char, bool>() }));
}

TEST(read_vector_with_true_char_function, push_back_by_default)
{
	vector<bool> expectedVector = { false, true, false, true, false, false, true };
	stringstream is(".#%#X@#");
	CInput input(is);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(booleans,
		{ vector<char>(), ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, '#', unordered_map<char, bool>() }));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_push_front)
{
	vector<bool> expectedVector = { true, false, false, true, false, true, false };
	stringstream is(".#.#..#");
	CInput input(is);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(booleans,
		{ vector<char>(), ReadVectorMethod::PUSH_FRONT, ReadLimit::UNLIMITED, '#', unordered_map<char, bool>() }));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	vector<bool> expectedVector = { false, false, false, false };
	stringstream is("    \n\n\n   .#.#..#");
	CInput input(is);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(booleans,
		{ vector<char>(), ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, '#', unordered_map<char, bool>() }));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_skip_whitespaces_and_empty_lines_but_does_not_do_that_at_beginning)
{
	vector<bool> expectedVector = { false, false, true, false, true, false, false, true };
	stringstream is("    \n\n\n   .#.#..#");
	CInput input(is);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(booleans,
		{ { ' ', '\n' }, ReadVectorMethod::PUSH_BACK, ReadLimit::UNLIMITED, '#', unordered_map<char, bool>() }));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_have_limit)
{
	vector<bool> expectedVector = { false, true, false };
	stringstream is(".#.#..#");
	CInput input(is);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
		booleans, { vector<char>(), ReadVectorMethod::PUSH_BACK, 3, '#', unordered_map<char, bool>() }));
	EXPECT_EQ(booleans, expectedVector);
}
