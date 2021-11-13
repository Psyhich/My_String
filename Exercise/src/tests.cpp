#include <gtest/gtest.h>
#include <cstring>

#include "my_string.h"

class CMyStringFixture : public ::testing::Test
{
protected:
	Exercise_1::CMyString myString;
	Exercise_1::CMyString secondString;

	const size_t nTestDataLength{17};
	const char szTestData[17]{"Some test string"};

	const size_t nSecondStringLength{19};
	const char szSecondTestString[19]{"Second test string"};

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
	myString.Insert(secondString.data(), 3);

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
	size_t nNewSize = myString.size() + secondString.size() - 1;
	myString.Insert(secondString.data(), 0);

	ASSERT_EQ(nNewSize, myString.size());

	char *szRightInserted = new char[nNewSize];
	szRightInserted[0] = '\0';
	std::strcat(szRightInserted, secondString.data());
	std::strcat(szRightInserted, szTestData);

	ASSERT_STREQ(myString.data(), szRightInserted);

	delete[] szRightInserted;
}

TEST_F(CMyStringFixture, EndInsertionTest)
{
	// Testing this with append function because it's already tested
	Exercise_1::CMyString copyString = myString + secondString;
	
	myString.Insert(secondString.data(), myString.size() - 1);

	ASSERT_EQ(copyString.size(), myString.size());

	ASSERT_STREQ(myString.data(), copyString.data());
}


TEST_F(CMyStringFixture, MultipleInsertionTest)
{
	for(size_t nTimesToInsert = 0; nTimesToInsert <= 5; nTimesToInsert++)
	{
		size_t nNewSize = myString.size() + (secondString.size() - 1) * nTimesToInsert;
		Exercise_1::CMyString newString = myString;
		newString.Insert(secondString.data(), 3, nTimesToInsert);

		ASSERT_EQ(nNewSize, newString.size());

		char *szRightInserted = new char[nNewSize];
		std::strncpy(szRightInserted, szTestData, 3);
		szRightInserted[3] = '\0';
		for(size_t nCount = 0; nCount < nTimesToInsert; nCount++)
		{
			std::strcat(szRightInserted, secondString.data());
		}
		std::strcat(szRightInserted, szTestData + 3);

		ASSERT_STREQ(newString.data(), szRightInserted);

		delete[] szRightInserted;
	}
}
