#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_vector_with_true_char_function, returns_false_if_can_not_read_any_element)
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
					'#',
					std::unordered_map<wchar_t, bool>()
			}
	));
}

TEST(read_vector_with_true_char_function, push_back_by_default)
{
	vector<bool> expectedVector = {false, true, false, true, false, false, true};
	wstringstream wis(L".#%#X@#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					'#',
					std::unordered_map<wchar_t, bool>()
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_push_front)
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
					'#',
					std::unordered_map<wchar_t, bool>()
			}));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	vector<bool> expectedVector = {false, false, false, false};
	wstringstream wis(L"    \n\n\n   .#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					'#',
					std::unordered_map<wchar_t, bool>()
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_skip_whitespaces_and_empty_lines_but_does_not_do_that_at_beginning)
{
	vector<bool> expectedVector = {false, false, true, false, true, false, false, true};
	wstringstream wis(L"    \n\n\n   .#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					{' ', '\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					'#',
					std::unordered_map<wchar_t, bool>()
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}

TEST(read_vector_with_true_char_function, can_have_limit)
{
	vector<bool> expectedVector = {false, true, false};
	wstringstream wis(L".#.#..#");
	CInput input(wis);
	vector<bool> booleans;
	EXPECT_TRUE(input.ReadVector(
			booleans,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					3,
					'#',
					std::unordered_map<wchar_t, bool>()
			}
	));
	EXPECT_EQ(booleans, expectedVector);
}
