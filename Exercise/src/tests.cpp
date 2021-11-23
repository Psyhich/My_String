#include <gtest/gtest.h>
#include <cstring>

#include "my_string.h"

#define TEST_DATA_LENGTH 17
#define SECOND_TEST_DATA_LENGTH 19

class CMyStringFixture : public ::testing::Test
{
protected:
	Exercise_1::CMyString myString;
	Exercise_1::CMyString secondString;

	const size_t nTestDataLength{TEST_DATA_LENGTH};
	const char szTestData[TEST_DATA_LENGTH]{"Some test string"};

	const size_t nSecondStringLength{SECOND_TEST_DATA_LENGTH};
	const char szSecondTestString[SECOND_TEST_DATA_LENGTH]{"Second test string"};

public:
	void SetUp() override
	{
		myString = Exercise_1::CMyString(szTestData);
		secondString = Exercise_1::CMyString(szSecondTestString);
	}
};

TEST_F(CMyStringFixture, DataCorrectnesTest) 
{
	ASSERT_EQ(nTestDataLength, myString.size());
	ASSERT_STREQ(szTestData, myString.data());
	ASSERT_EQ(szTestData[nTestDataLength - 1], '\0');
}

TEST_F(CMyStringFixture, DataCopyTest)
{
	Exercise_1::CMyString copiedString{myString};
	ASSERT_EQ(myString.size(), copiedString.size());
	ASSERT_STREQ(myString.data(), copiedString.data());

	Exercise_1::CMyString operatorCopiedString = myString;
	ASSERT_EQ(myString.size(), operatorCopiedString.size());
	ASSERT_STREQ(myString.data(), operatorCopiedString.data());

	Exercise_1::CMyString operatorCopiedChars = szTestData;
	ASSERT_EQ(myString.size(), operatorCopiedChars.size());
	ASSERT_STREQ(myString.data(), operatorCopiedChars.data());
}

TEST_F(CMyStringFixture, AdditionOperatorTest)
{
	Exercise_1::CMyString concatenatedString = myString + secondString; 
	ASSERT_EQ(concatenatedString.size(), myString.size() + secondString.size() - 1);

	char* pszConcatenatedChars = new char[nTestDataLength + nSecondStringLength - 1];
	pszConcatenatedChars[0] = '\0';
	std::strcat(pszConcatenatedChars, szTestData);
	std::strcat(pszConcatenatedChars, szSecondTestString);

	ASSERT_STREQ(concatenatedString.data(), pszConcatenatedChars);
	delete[] pszConcatenatedChars;
}

TEST_F(CMyStringFixture, InsertionTest)
{
	size_t nNewSize = myString.size() + secondString.size() - 1;
	myString.Insert(secondString, 3);

	ASSERT_EQ(nNewSize, myString.size());

	char *szRightInserted = new char[nNewSize];
	std::strncpy(szRightInserted, szTestData, 3);
	szRightInserted[3] = '\0';
	std::strcat(szRightInserted, secondString.data());
	std::strcat(szRightInserted, szTestData + 3);

	ASSERT_STREQ(myString.data(), szRightInserted);

	delete[] szRightInserted;
}

TEST_F(CMyStringFixture, BeginInsertionTest)
{
	Exercise_1::CMyString copyString = secondString + myString;
	myString.Insert(secondString, 0);

	ASSERT_EQ(copyString.size(), myString.size());

	ASSERT_STREQ(myString.data(), copyString.data());
}

TEST_F(CMyStringFixture, EndInsertionTest)
{
	// Testing this with append function because it's already tested
	Exercise_1::CMyString copyString = myString + secondString;
	
	myString.Insert(secondString, myString.size() - 1);

	ASSERT_EQ(copyString.size(), myString.size());

	ASSERT_STREQ(myString.data(), copyString.data());
}

TEST_F(CMyStringFixture, PartialInsertionTest)
{
	Exercise_1::CMyString str = myString;
	constexpr const int ciInsertIndex = 2;
	str.Insert(secondString, ciInsertIndex, 4);

	ASSERT_EQ(str.size(), myString.size() + 4);

	char *szRightInserted = new char[str.size()]; // We can use size here because we checked it
	std::strncpy(szRightInserted, szTestData, ciInsertIndex);
	szRightInserted[ciInsertIndex] = '\0';
	std::strncat(szRightInserted, secondString.data(), 4);
	std::strcat(szRightInserted, szTestData + ciInsertIndex);

	ASSERT_STREQ(str.data(), szRightInserted);

	delete[] szRightInserted;

}

TEST(CMyStringTest, EqualityTest)
{
	Exercise_1::CMyString str1 = "Hello world";
	Exercise_1::CMyString str2 = "Hello world";
	
	ASSERT_EQ(str1 == str2, true);

	str2[0] = 'a';
	ASSERT_EQ(str1 == str2, false);

	str2 = str2 + "Hi";

	ASSERT_EQ(str1 == str2, false);

	Exercise_1::CMyString nullPtrString = nullptr;
	ASSERT_EQ(nullPtrString == str1, false);
	ASSERT_EQ(nullPtrString == nullPtrString, true);



	str1 = "Same other1";
	ASSERT_EQ(str1 == str2, false);
	
}

TEST(CMyStringFailTests, NullptrConstructor)
{
	Exercise_1::CMyString nullptrStr{nullptr};

	ASSERT_EQ(nullptrStr.size(), 0);
	ASSERT_EQ(nullptrStr.data(), nullptr);
}

TEST(CMyStringFailTests, NullptrAssignOperator)
{
	Exercise_1::CMyString nullptrStr = nullptr;

	ASSERT_EQ(nullptrStr.size(), 0);
	ASSERT_EQ(nullptrStr.data(), nullptr);
}

TEST(CMyStringFailTests, NullptrAppends)
{
	Exercise_1::CMyString basicString{"Hello"};
	Exercise_1::CMyString safeCopy{"Hello"};
	Exercise_1::CMyString nullptrStr = nullptr;

	basicString = basicString + nullptrStr;

	ASSERT_EQ(basicString.size(), safeCopy.size());
	ASSERT_STREQ(basicString.data(), safeCopy.data());

	basicString = nullptr;

	basicString = basicString + nullptrStr;
	ASSERT_EQ(basicString.size(), 0);
	ASSERT_EQ(basicString.data(), nullptr);
}

TEST(CMyStringFailTests, NullptrInsert)
{
	Exercise_1::CMyString basicString{"Hello world"};
	Exercise_1::CMyString stringCopy{basicString};
	const size_t nSizeBefore = basicString.size();
	basicString.Insert(nullptr, 1);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());
}

TEST(CMyStringFailTests, WrongInsertionTest)
{
	Exercise_1::CMyString basicString{"Hello world"};
	Exercise_1::CMyString stringCopy{basicString};
	const size_t nSizeBefore = basicString.size();
	basicString.Insert("Some string", 100);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());

	basicString.Insert("Some other string", 1, 100);

	ASSERT_EQ(basicString.size(), nSizeBefore);
	ASSERT_STREQ(basicString.data(), stringCopy.data());
}
