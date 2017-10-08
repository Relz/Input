#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(read_vector_wchar_function, returns_false_if_can_not_read_any_element)
{
	wstringstream wis(L"0");
	CInput input(wis);
	int arg0;
	vector<wchar_t> wChars;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(wChars));
}

TEST(read_vector_wchar_function, reads_by_wchar_and_push_back_by_default)
{
	vector<wchar_t> expectedVector = {'0', ' ', '1', ' ', '2', '3'};
	wstringstream wis(L"0 1 23");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_TRUE(input.ReadVector(wChars));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_push_front)
{
	vector<wchar_t> expectedVector = {'3', '2', '1', '0'};
	wstringstream wis(L"0123");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_TRUE(input.ReadVector(
			wChars,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_FRONT,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, wchar_t>()
			}
	));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	vector<wchar_t> expectedVector = {};
	wstringstream wis(L"\n     \n\n\n             .#.#..#");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_FALSE(input.ReadVector(wChars));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, does_not_beginning_with_skipping)
{
	vector<wchar_t> expectedVector = {' ', '.', '#', '.', '#', '.', '.', '#'};
	wstringstream wis(L"     \n\n\n             .#.#..#");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_TRUE(input.ReadVector(
			wChars,
			{
					{' ', '\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, wchar_t>()
			}
	));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_skip_whitespaces_and_empty_lines)
{
	vector<wchar_t> expectedVector = {'.', '#', '.', '#', '.', '.', '#'};
	wstringstream wis(L".#     \n\n\n             .#..#");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_TRUE(input.ReadVector(
			wChars,
			{
					{' ', '\n'},
					ReadVectorMethod::PUSH_BACK,
					ReadLimit::UNLIMITED,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, wchar_t>()
			}
	));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_have_limit)
{
	vector<wchar_t> expectedVector = {'.', '#', '.'};
	wstringstream wis(L".#.#..#");
	CInput input(wis);
	vector<wchar_t> wChars;
	EXPECT_TRUE(input.ReadVector(
			wChars,
			{
					std::vector<wchar_t>(),
					ReadVectorMethod::PUSH_BACK,
					3,
					NOT_A_CHARACTER,
					std::unordered_map<wchar_t, wchar_t>()
			}
	));
	EXPECT_EQ(wChars, expectedVector);
}
