#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(scan, returns_false_if_end_of_stream_scanned_otherwise_string_and_delimiter)
{
	wstringstream stringStream(L"class User");
	Input input(stringStream);
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_TRUE(input.Scan({ L" " }, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, L"class");
		EXPECT_EQ(delimiterStreamString.string, L" ");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 6);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 7, true, L'U', false, false));
	}
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_TRUE(input.Scan({ L" " }, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, L"User");
		EXPECT_EQ(delimiterStreamString.string, L"");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 7);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 11, false, 0, false, true));
	}
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_FALSE(input.Scan({ L" " }, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, L"");
		EXPECT_EQ(delimiterStreamString.string, L"");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 1);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 11, false, 0, false, true));
	}
}

TEST(scan, can_work_with_multiple_lines_and_delimiters)
{
	wstringstream stringStream(L"class User\n{\n\tConstructor(String name)\n\t{ }\n}");
	Input input(stringStream);

	vector<wstring> scannedStrings;
	vector<wstring> expectedScannedStrings {
		L"class", L"User", L"", L"", L"", L"Constructor", L"String", L"name", L"", L"", L"", L"", L"", L"", L""
	};
	vector<wstring> scannedDelimiters;
	vector<wstring> expectedScannedDelimiters {
		L" ", L"\n", L"{", L"\n", L"\t", L"(", L" ", L")", L"\n", L"\t", L"{", L" ", L"}", L"\n", L"}"
	};
	StreamString scannedStreamString;
	StreamString delimiterStreamString;
	vector<wstring> delimiters { L" ", L"(", L")", L"{", L"}", L";", L"\t", L"\n" };
	while (input.Scan(delimiters, scannedStreamString, delimiterStreamString))
	{
		scannedStrings.push_back(scannedStreamString.string);
		scannedDelimiters.push_back(delimiterStreamString.string);
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
	EXPECT_TRUE(TestHelper::CheckState(input, 5, 2, false, 0, false, true));
}

TEST(scan, can_work_with_big_delimiter)
{
	wstringstream stringStream(L"user-->name user->name user--->name");
	Input input(stringStream);

	vector<wstring> scannedStrings;
	vector<wstring> expectedScannedStrings { L"user", L"name", L"user->name", L"user-", L"name" };
	vector<wstring> scannedDelimiters;
	vector<wstring> expectedScannedDelimiters { L"-->", L" ", L" ", L"-->", L"" };
	StreamString scannedStreamString;
	StreamString delimiterStreamString;
	while (input.Scan({ L" ", L"-->" }, scannedStreamString, delimiterStreamString))
	{
		scannedStrings.push_back(scannedStreamString.string);
		scannedDelimiters.push_back(delimiterStreamString.string);
		delimiterStreamString.string.clear();
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 36, false, 0, false, true));
}
