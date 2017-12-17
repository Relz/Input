#include <sstream>
#include "gtest/gtest.h"
#include "../../Input/Input.h"

using namespace std;

TEST(scan_function, returns_false_if_end_of_stream)
{
	istringstream is("class User");
	CInput input(is);
	string scanned;
	string delimiter;
	EXPECT_TRUE(input.Scan(scanned, delimiter, {" "}));
	EXPECT_TRUE(input.Scan(scanned, delimiter, {" "}));
	EXPECT_FALSE(input.Scan(scanned, delimiter, {" "}));
}

TEST(scan_function, returns_scanned_string_and_delimiter)
{
	istringstream is("class User");
	CInput input(is);
	string scanned;
	string delimiter;
	EXPECT_TRUE(input.Scan(scanned, delimiter, {" "}));
	EXPECT_EQ(scanned, "class");
	EXPECT_EQ(delimiter, " ");
	EXPECT_TRUE(input.Scan(scanned, delimiter, {" "}));
	EXPECT_EQ(scanned, "User");
	EXPECT_EQ(delimiter, "");
	EXPECT_FALSE(input.Scan(scanned, delimiter, {" "}));
	EXPECT_EQ(scanned, "");
	EXPECT_EQ(delimiter, "");
}

TEST(scan_function, can_work_with_multiple_lines_and_delimiters)
{
	vector<string> expectedScannedWords = {"class", "User", "", "Constructor", "String", "name", "", "", "", "", ""};
	vector<string> expectedScannedDelimiters = {" ", "{", "\t", "(", " ", ")", "\t", "{", " ", "}", "}"};
	istringstream is("class User\n{\n\tConstructor(String name)\n\t{ }\n}");
	CInput input(is);
	vector<string> scannedWords;
	vector<string> scannedDelimiters;
	string scanned;
	string delimiter;
	while (input.Scan(scanned, delimiter, {" ", "(", ")", "{", "}", ";", "\t"}))
	{
		scannedWords.push_back(scanned);
		scannedDelimiters.push_back(delimiter);
	}
	EXPECT_EQ(scannedWords, expectedScannedWords);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}

TEST(scan_function, can_work_with_big_delimiter)
{
	vector<string> expectedScannedWords = {"user", "name", "user->name", "user-", "name"};
	vector<string> expectedScannedDelimiters = {"-->", " ", " ", "-->", ""};
	istringstream is("user-->name user->name user--->name");
	CInput input(is);
	vector<string> scannedWords;
	vector<string> scannedDelimiters;
	string scanned;
	string delimiter;
	while (input.Scan(scanned, delimiter, {" ", "-->"}))
	{
		scannedWords.push_back(scanned);
		scannedDelimiters.push_back(delimiter);
	}
	EXPECT_EQ(scannedWords, expectedScannedWords);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}
